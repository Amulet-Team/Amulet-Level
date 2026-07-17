#include <chrono>
#include <mutex>
#include <shared_mutex>

#include "level.hpp"

namespace Amulet {

JavaLevelOpenData::JavaLevelOpenData()
    : history_enabled(std::make_shared<bool>(true))
{
}

JavaLevelOpenData& JavaLevel::_get_open_data()
{
    if (!_open_data) {
        throw std::runtime_error("The level is not open.");
    }
    return *_open_data;
}

JavaLevel::JavaLevel(std::unique_ptr<JavaRawLevel> raw_level)
    : _raw_level(std::move(raw_level))
    , _level_name_changed_token(_raw_level->level_name_changed.connect([this](std::string level_name) { level_name_changed.dispatch(std::move(level_name)); }))
    , _on_open_token(_raw_level->opened.connect([this]() { _on_open(); }))
    , _on_close_token(_raw_level->closed.connect([this]() { _on_close(); }))
    , _on_reload_token(_raw_level->reloaded.connect([this]() { _on_reload(); }))
{
}

JavaLevel::~JavaLevel()
{
    _raw_level->level_name_changed.disconnect(_level_name_changed_token);
    _raw_level->opened.disconnect(_on_open_token);
    _raw_level->closed.disconnect(_on_close_token);
    _raw_level->reloaded.disconnect(_on_reload_token);
    close();
}

std::unique_ptr<JavaLevel> JavaLevel::load(const std::filesystem::path& path)
{
    return std::unique_ptr<JavaLevel>(new JavaLevel(JavaRawLevel::load(path)));
}

std::unique_ptr<JavaLevel> JavaLevel::create(const JavaCreateArgsV1& args)
{
    return std::unique_ptr<JavaLevel>(new JavaLevel(JavaRawLevel::create(args)));
}

bool JavaLevel::is_open()
{
    std::shared_lock lock(_open_data_mutex);
    return bool(_open_data);
}

const std::string JavaLevel::get_platform()
{
    return _raw_level->get_platform();
}

VersionNumber JavaLevel::get_max_game_version()
{
    OrderedLockGuard<ThreadAccessMode::Read, ThreadShareMode::SharedReadWrite> lock(_raw_level->get_mutex());
    return _raw_level->get_data_version();
}

VersionNumber JavaLevel::get_max_block_version()
{
    return get_max_game_version();
}

bool JavaLevel::is_supported()
{
    return _raw_level->is_supported();
}

PIL::Image::Image JavaLevel::get_thumbnail()
{
    return _raw_level->get_thumbnail();
};

const std::string JavaLevel::get_level_name()
{
    OrderedLockGuard<ThreadAccessMode::Read, ThreadShareMode::SharedReadWrite> lock(_raw_level->get_mutex());
    return _raw_level->get_level_name();
}

std::chrono::system_clock::time_point JavaLevel::get_modified_time()
{
    OrderedLockGuard<ThreadAccessMode::Read, ThreadShareMode::SharedReadWrite> lock(_raw_level->get_mutex());
    return _raw_level->get_modified_time();
}

size_t JavaLevel::get_sub_chunk_size()
{
    return 16;
}

const std::filesystem::path& JavaLevel::get_path()
{
    return _raw_level->get_path();
}

void JavaLevel::_on_open()
{
    {
        std::lock_guard lock(_open_data_mutex);
        if (_open_data) {
            throw std::runtime_error("Level open state desynced from raw level.");
        }
        _open_data = std::make_unique<JavaLevelOpenData>();
    }
    opened.dispatch();
}

void JavaLevel::open()
{
    OrderedLockGuard<ThreadAccessMode::ReadWrite, ThreadShareMode::Unique> lock(_raw_level->get_mutex());
    _raw_level->open();
}

void JavaLevel::purge()
{
    {
        std::shared_lock lock1(_open_data_mutex);
        auto& open_data = _get_open_data();
        std::lock_guard lock2(open_data.history_manager.get_mutex());
        open_data.history_manager.reset();
    }
    purged.dispatch();
    history_changed.dispatch();
}

void JavaLevel::save()
{
    for (const auto& dimension_id : get_dimension_ids()) {
        auto dimension = get_java_dimension(dimension_id);
        dimension->save();
    }
}

void JavaLevel::_on_close()
{
    {
        std::lock_guard lock(_open_data_mutex);
        if (!_open_data) {
            throw std::runtime_error("Level open state desynced from raw level.");
        }
        _open_data = nullptr;
    }
    closed.dispatch();
}

void JavaLevel::close()
{
    OrderedLockGuard<ThreadAccessMode::ReadWrite, ThreadShareMode::Unique> lock(_raw_level->get_mutex());
    _raw_level->close();
}

void JavaLevel::create_restore_point()
{
    {
        std::shared_lock lock1(_open_data_mutex);
        auto& open_data = _get_open_data();
        std::lock_guard lock2(open_data.history_manager.get_mutex());
        open_data.history_manager.create_undo_bin();
    }
    history_changed.dispatch();
}

size_t JavaLevel::get_undo_count()
{
    std::shared_lock lock1(_open_data_mutex);
    auto& open_data = _get_open_data();
    std::shared_lock lock2(open_data.history_manager.get_mutex());
    return open_data.history_manager.get_undo_count();
}

void JavaLevel::undo()
{
    {
        std::shared_lock lock1(_open_data_mutex);
        auto& open_data = _get_open_data();
        std::lock_guard lock2(open_data.history_manager.get_mutex());
        open_data.history_manager.undo();
    }
    history_changed.dispatch();
}

size_t JavaLevel::get_redo_count()
{
    std::shared_lock lock1(_open_data_mutex);
    auto& open_data = _get_open_data();
    std::shared_lock lock2(open_data.history_manager.get_mutex());
    return open_data.history_manager.get_redo_count();
}

void JavaLevel::redo()
{
    {
        std::shared_lock lock1(_open_data_mutex);
        auto& open_data = _get_open_data();
        std::lock_guard lock2(open_data.history_manager.get_mutex());
        open_data.history_manager.redo();
    }
    history_changed.dispatch();
}

bool JavaLevel::get_history_enabled()
{
    std::shared_lock lock(_open_data_mutex);
    return *_get_open_data().history_enabled;
}

void JavaLevel::set_history_enabled(bool history_enabled)
{
    std::shared_lock lock(_open_data_mutex);
    *_get_open_data().history_enabled = history_enabled;
    history_enabled_changed.dispatch();
}

std::vector<std::string> JavaLevel::get_dimension_ids()
{
    OrderedLockGuard<ThreadAccessMode::Read, ThreadShareMode::SharedReadWrite> lock(_raw_level->get_mutex());
    return _raw_level->get_dimension_ids();
}

std::shared_ptr<JavaDimension> JavaLevel::get_java_dimension(const DimensionId& dimension_id)
{
    std::shared_lock lock(_open_data_mutex);
    auto& open_data = _get_open_data();
    {
        // Find the dimension with a shared lock.
        std::shared_lock dimensions_lock(open_data.dimensions_mutex);
        auto it = open_data.dimensions.find(dimension_id);
        if (it != open_data.dimensions.end()) {
            return it->second;
        }
    }
    {
        // If it doesn't exist try again with a unique lock.
        std::lock_guard dimensions_lock(open_data.dimensions_mutex);
        auto it = open_data.dimensions.find(dimension_id);
        if (it != open_data.dimensions.end()) {
            return it->second;
        }
        OrderedLockGuard<
            ThreadAccessMode::Read,
            ThreadShareMode::SharedReadWrite>
            raw_level_lock(_raw_level->get_mutex());
        auto raw_dimension = _raw_level->get_dimension(dimension_id);
        auto dimension = std::shared_ptr<JavaDimension>(new JavaDimension(
            raw_dimension,
            open_data.history_manager,
            open_data.history_enabled));
        open_data.dimensions.emplace(raw_dimension->get_dimension_id(), dimension);
        open_data.dimensions.emplace(raw_dimension->get_relative_path(), dimension);
        return dimension;
    }
}

std::shared_ptr<Dimension> JavaLevel::get_dimension(const DimensionId& dimension_id)
{
    return get_java_dimension(dimension_id);
}

void JavaLevel::compact()
{
    OrderedLockGuard<ThreadAccessMode::Read, ThreadShareMode::SharedReadWrite> lock(_raw_level->get_mutex());
    _raw_level->compact();
}

void JavaLevel::reload_metadata()
{
    std::lock_guard lock(_raw_level->get_mutex());
    _raw_level->reload_metadata();
}

void JavaLevel::_on_reload()
{
    {
        std::lock_guard lock1(_open_data_mutex);
        // purge loaded data.
        auto& open_data = _get_open_data();
        std::lock_guard lock2(open_data.history_manager.get_mutex());
        open_data.history_manager.reset();
    }
    reloaded.dispatch();
    history_changed.dispatch();
}

void JavaLevel::reload()
{
    // reload the raw level.
    std::lock_guard lock(_raw_level->get_mutex());
    _raw_level->reload();
}

JavaRawLevel& JavaLevel::get_raw_level()
{
    return *_raw_level;
}

} // namespace Amulet
