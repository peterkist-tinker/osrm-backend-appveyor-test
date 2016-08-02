#ifndef EXTRACTION_CONTAINERS_HPP
#define EXTRACTION_CONTAINERS_HPP

#include "extractor/external_memory_node.hpp"
#include "extractor/first_and_last_segment_of_way.hpp"
#include "extractor/guidance/turn_lane_types.hpp"
#include "extractor/internal_extractor_edge.hpp"
#include "extractor/restriction.hpp"
#include "extractor/scripting_environment.hpp"

#include <cstdint>
#include <stxxl/vector>
#include <unordered_map>

namespace osrm
{
namespace extractor
{

/**
 * Uses external memory containers from stxxl to store all the data that
 * is collected by the extractor callbacks.
 *
 * The data is the filtered, aggregated and finally written to disk.
 */
class ExtractionContainers
{
#ifndef _MSC_VER
    constexpr static unsigned stxxl_memory =
        ((sizeof(std::size_t) == 4) ? std::numeric_limits<int>::max()
                                    : std::numeric_limits<unsigned>::max());
#else
    const static unsigned stxxl_memory = ((sizeof(std::size_t) == 4) ? INT_MAX : UINT_MAX);
#endif
    void PrepareNodes();
    void PrepareRestrictions();
    void PrepareEdges(lua_State *segment_state);

    void WriteNodes(std::ofstream &file_out_stream) const;
    void WriteRestrictions(const std::string &restrictions_file_name) const;
    void WriteEdges(std::ofstream &file_out_stream) const;
    void WriteCharData(const std::string &file_name);
    void
    WriteTurnLaneMasks(const std::string &file_name,
                       const stxxl::vector<std::uint32_t> &turn_lane_offsets,
                       const stxxl::vector<guidance::TurnLaneType::Mask> &turn_lane_masks) const;

  public:
    using STXXLNodeIDVector = stxxl::vector<OSMNodeID>;
    using STXXLNodeVector = stxxl::vector<ExternalMemoryNode>;
    using STXXLEdgeVector = stxxl::vector<InternalExtractorEdge>;
    using STXXLRestrictionsVector = stxxl::vector<InputRestrictionContainer>;
    using STXXLWayIDStartEndVector = stxxl::vector<FirstAndLastSegmentOfWay>;
    using STXXLNameCharData = stxxl::vector<unsigned char>;
    using STXXLNameOffsets = stxxl::vector<unsigned>;

    STXXLNodeIDVector used_node_id_list;
    STXXLNodeVector all_nodes_list;
    STXXLEdgeVector all_edges_list;
    STXXLNameCharData name_char_data;
    STXXLNameOffsets name_offsets;
    // an adjacency array containing all turn lane masks
    stxxl::vector<std::uint32_t> turn_lane_offsets;
    stxxl::vector<guidance::TurnLaneType::Mask> turn_lane_masks;
    STXXLRestrictionsVector restrictions_list;
    STXXLWayIDStartEndVector way_start_end_id_list;
    std::unordered_map<OSMNodeID, NodeID> external_to_internal_node_id_map;
    unsigned max_internal_node_id;

    ExtractionContainers();

    void PrepareData(const std::string &output_file_name,
                     const std::string &restrictions_file_name,
                     const std::string &names_file_name,
                     const std::string &turn_lane_file_name,
                     lua_State *segment_state);
};
}
}

#endif /* EXTRACTION_CONTAINERS_HPP */
