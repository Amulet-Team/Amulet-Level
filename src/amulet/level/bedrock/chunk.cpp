#include <string>
#include <map>
#include <memory>
#include <functional>
#include <stdexcept>

//#include <amulet/core/chunk/chunk.hpp>
//#include <amulet/core/chunk/component/block_component.hpp>

#include "chunk.hpp"

namespace Amulet {

static std::map<std::string, std::function<std::unique_ptr<BedrockChunk>()>> bedrock_chunk_constructors = {
};

namespace detail {
    std::unique_ptr<BedrockChunk> get_bedrock_null_chunk(const std::string& chunk_id)
    {
        auto it = bedrock_chunk_constructors.find(chunk_id);
        if (it == bedrock_chunk_constructors.end()) {
            throw std::runtime_error("Unknown chunk_id " + chunk_id);
        }
        return it->second();
    }
    std::string get_bedrock_chunk_id(const BedrockChunk& chunk) {
        std::string chunk_id = chunk.get_chunk_id();
        if (!bedrock_chunk_constructors.contains(chunk_id)) {
            throw std::runtime_error("Unknown chunk_id " + chunk_id);
        }
        return chunk_id;
    }
} // namespace detail

} // namespace Amulet
