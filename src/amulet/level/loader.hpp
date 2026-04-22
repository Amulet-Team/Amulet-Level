#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>

#include <amulet/level/abc/level.hpp>
#include <amulet/level/dll.hpp>

namespace Amulet {

class LevelLoaderToken {
public:
    virtual ~LevelLoaderToken() = default;
    virtual std::string repr() const = 0;
    virtual size_t hash() const = 0;
    virtual bool operator==(const LevelLoaderToken&) const = 0;
};

class AMULET_LEVEL_EXPORT LevelLoaderPathToken : public LevelLoaderToken {
public:
    std::filesystem::path path;
    LevelLoaderPathToken(std::filesystem::path path);
    LevelLoaderPathToken(const LevelLoaderPathToken& token) = default;
    std::string repr() const override;
    size_t hash() const override;
    bool operator==(const LevelLoaderToken&) const override;
};

class AMULET_LEVEL_EXPORT LevelLoader {
public:
    // The name of the loader.
    std::string name;
    // The function to load the level.
    std::function<std::unique_ptr<Level>(const LevelLoaderToken&)> loader;

    LevelLoader(
        const std::string& name,
        std::function<std::unique_ptr<Level>(const LevelLoaderToken&)> loader);
};

}

template <>
struct std::hash<Amulet::LevelLoaderToken> {
    size_t operator()(const Amulet::LevelLoaderToken& token) const noexcept;
};

namespace Amulet {

class AMULET_LEVEL_EXPORT LevelLoaderRegister {
private:
    std::shared_ptr<LevelLoader> _loader;

public:
    LevelLoaderRegister(std::shared_ptr<LevelLoader>);
    ~LevelLoaderRegister();
};

class AMULET_LEVEL_EXPORT NoValidLevelLoader : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
    ~NoValidLevelLoader() noexcept override;
};

AMULET_LEVEL_EXPORT std::shared_ptr<Level> get_level(std::shared_ptr<LevelLoaderToken>);
AMULET_LEVEL_EXPORT std::shared_ptr<Level> get_level(std::filesystem::path);

} // namespace Amulet
