/**********************************************************************
Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
********************************************************************/

#include "graph_scheme.h"

////////////////////////////////////////////////
//// GraphScheme implemenataion
////////////////////////////////////////////////

namespace {
    struct GraphSchemeConcrete : public GraphScheme
    {
        GraphSchemeConcrete(UberTree::Ptr tree, RadeonRays::int2 root_pos) :
            GraphScheme(tree, root_pos)
        {   }
    };
}

GraphScheme::GraphScheme(UberTree::Ptr tree, RadeonRays::int2 root_pos) : m_is_dirty(false)
{
    if (!tree)
        throw std::logic_error(
            "GraphScheme::GraphScheme(...): 'tree' is nullptr");

    m_trees.push_back(tree);
}

GraphScheme::Ptr GraphScheme::Create(UberTree::Ptr tree, RadeonRays::int2 root_pos)
{
    return std::make_shared<GraphScheme>(
        GraphSchemeConcrete(tree, root_pos));
}

void GraphScheme::RecomputeCoordinates(RadeonRays::int2 root_pos)
{
    const int x_offset = 120;
    const int y_offset = 60;
    const RadeonRays::int2 node_size = RadeonRays::int2(100, 70);
    // primary tree
    auto iter = UberTreeIterator::Create(m_trees[0]);

    int level = 0;
    int level_item_counter = 0;
    while (iter->IsValid())
    {
        level_item_counter = (iter->GetLevel() == level) ?
            (level_item_counter + 1) : (0);
        level = iter->GetLevel();

        RadeonRays::int2 position(
            root_pos.x + level * x_offset,
            root_pos.y + level_item_counter * y_offset);

        m_nodes.push_back(Node(iter->Item(), position, node_size));

        if (iter->Item()->GetParentId() >= 0)
        {
            m_links.push_back(
                Link(iter->Item()->GetParentId(),
                     iter->Item()->GetId()));
        }

        iter->Next();
    }
}

void GraphScheme::UpdateNodePos(int id, RadeonRays::int2 pos)
{
    throw std::runtime_error("Not implemented");
}

////////////////////////////////////////////////
//// Node implemenataion
////////////////////////////////////////////////

GraphScheme::Node::Node(
    std::uint32_t id_,
    std::string name_,
    RadeonRays::int2 pos_,
    RadeonRays::int2 size_) :
    id(id_), name(name_), pos(pos_), size(size_)
{   }

GraphScheme::Node::Node(
    UberNode::Ptr node_,
    RadeonRays::int2 pos_,
    RadeonRays::int2 size_) :
    id(node_->GetId()), pos(pos_), size(size_)
{
    name = "Don't forget about me";
}