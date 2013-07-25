
/*LICENSE_START*/
/*
 * Copyright 2013 Washington University,
 * All rights reserved.
 *
 * Connectome DB and Connectome Workbench are part of the integrated Connectome
 * Informatics Platform.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the names of Washington University nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
/*LICENSE_END*/

#define __CONNECTIVITY_DATA_LOADED_DECLARE__
#include "ConnectivityDataLoaded.h"
#undef __CONNECTIVITY_DATA_LOADED_DECLARE__

#include "CaretAssert.h"
#include "SceneClassAssistant.h"
#include "SceneAttributes.h"
#include "SceneClass.h"
#include "ScenePrimitiveArray.h"
#include "SurfaceFile.h"

using namespace caret;



/**
 * \class caret::ConnectivityDataLoaded
 * \brief Maintains information on loaded brainordinate data.
 * \ingroup Brain
 */

/**
 * Constructor.
 */
ConnectivityDataLoaded::ConnectivityDataLoaded()
: CaretObject()
{
    reset();
    
    m_sceneAssistant = new SceneClassAssistant();
    
    m_sceneAssistant->add("m_surfaceNumberOfNodes",
                          &m_surfaceNumberOfNodes);
    m_sceneAssistant->add<StructureEnum, StructureEnum::Enum>("m_surfaceStructure",
                                                              &m_surfaceStructure);
    reset();
}

/**
 * Destructor.
 */
ConnectivityDataLoaded::~ConnectivityDataLoaded()
{
    reset();
    
    delete m_sceneAssistant;
}

/**
 * Reset the data.
 */
void
ConnectivityDataLoaded::reset()
{
    m_mode = MODE_NONE;
    
    m_surfaceNodeIndices.clear();
    m_surfaceNumberOfNodes = 0;
    m_surfaceStructure = StructureEnum::INVALID;
    
    m_voxelIndices.clear();
}

/**
 * @return The mode.
 */
ConnectivityDataLoaded::Mode
ConnectivityDataLoaded::getMode() const
{
    return m_mode;
}

/**
 * Get the surface loading information (MODE_SURFACE_NODE)
 *
 * @param structure
 *    The surface structure.
 * @param surfaceNumberOfNodes
 *    Number of nodes in surface.
 * @param surfaceNodeIndex
 *    Index of the surface node.
 */
void
ConnectivityDataLoaded::getSurfaceNodeLoading(StructureEnum::Enum& structure,
                                              int32_t& surfaceNumberOfNodes,
                                              int32_t& surfaceNodeIndex) const
{
    structure = m_surfaceStructure;
    surfaceNumberOfNodes = m_surfaceNumberOfNodes;
    if (m_surfaceNodeIndices.empty() == false) {
        surfaceNodeIndex = m_surfaceNodeIndices[0];
    }
    else {
        surfaceNodeIndex = -1;
    }
}

/**
 * Set the surface loading information (MODE_SURFACE_NODE)
 *
 * @param structure
 *    The surface structure.
 * @param surfaceNumberOfNodes
 *    Number of nodes in surface.
 * @param surfaceNodeIndex
 *    Index of the surface node.
 */
void
ConnectivityDataLoaded::setSurfaceNodeLoading(const StructureEnum::Enum structure,
                                              const int32_t surfaceNumberOfNodes,
                                              const int32_t surfaceNodeIndex)
{
    reset();
    
    m_mode = MODE_SURFACE_NODE;
    m_surfaceStructure = structure;
    m_surfaceNumberOfNodes = surfaceNumberOfNodes;
    m_surfaceNodeIndices.push_back(surfaceNodeIndex);
}

/**
 * Get the surface average node loading information (MODE_SURFACE_NODE_AVERAGE)
 *
 * @param structure
 *    The surface structure.
 * @param surfaceNumberOfNodes
 *    Number of nodes in surface.
 * @param surfaceNodeIndices
 *    Indices of the surface nodes.
 */
void
ConnectivityDataLoaded::getSurfaceAverageNodeLoading(StructureEnum::Enum& structure,
                                                     int32_t& surfaceNumberOfNodes,
                                                     std::vector<int32_t>& surfaceNodeIndices) const
{
    structure = m_surfaceStructure;
    surfaceNumberOfNodes = m_surfaceNumberOfNodes;
    surfaceNodeIndices = m_surfaceNodeIndices;
}

/**
 * Set the surface average node loading information (MODE_SURFACE_NODE_AVERAGE)
 *
 * @param structure
 *    The surface structure.
 * @param surfaceNumberOfNodes
 *    Number of nodes in surface.
 * @param surfaceNodeIndices
 *    Indices of the surface nodes.
 */
void
ConnectivityDataLoaded::setSurfaceAverageNodeLoading(const StructureEnum::Enum structure,
                                                     const int32_t surfaceNumberOfNodes,
                                                     const std::vector<int32_t>& surfaceNodeIndices)
{
    reset();
    
    m_mode = MODE_SURFACE_NODE_AVERAGE;
    m_surfaceStructure = structure;
    m_surfaceNumberOfNodes = surfaceNumberOfNodes;
    m_surfaceNodeIndices = surfaceNodeIndices;
}

/**
 * Get the voxel loading voxel IJK indices (MODE_VOXEL).
 *
 * @param voxelIndexIJK
 *    Indices of the voxel.
 */
void
ConnectivityDataLoaded::getVolumeVoxelLoading(VoxelIJK& voxelIndexIJK) const
{
    if (m_voxelIndices.empty() == false) {
        voxelIndexIJK = m_voxelIndices[0];
    }
    else {
        voxelIndexIJK.m_ijk[0] = -1;
        voxelIndexIJK.m_ijk[1] = -1;
        voxelIndexIJK.m_ijk[2] = -1;
    }
}

/**
 * Set the voxel loading voxel IJK indices (MODE_VOXEL)
 *
 * @param voxelIndexIJK
 *    Indices of the voxel.
 */
void ConnectivityDataLoaded::setVolumeVoxelLoading(const VoxelIJK& voxelIndexIJK)
{
    reset();
    
    m_mode = MODE_VOXEL;
    m_voxelIndices.push_back(voxelIndexIJK);
}

/**
 * Get the voxel average loading voxel IJK indices (MODE_VOXEL_AVERAGE)
 *
 * @param voxelIndicesIJK
 *    Indices of the voxels.
 */
void
ConnectivityDataLoaded::getVolumeAverageVoxelLoading(std::vector<VoxelIJK>& voxelIndicesIJK) const
{
    voxelIndicesIJK = m_voxelIndices;
}

/**
 * Set the voxel average loading voxel IJK indices (MODE_VOXEL_AVERAGE)
 *
 * @param voxelIndicesIJK
 *    Indices of the voxels.
 */
void
ConnectivityDataLoaded::setVolumeAverageVoxelLoading(const std::vector<VoxelIJK>& voxelIndicesIJK)
{
    reset();
    
    m_mode = MODE_VOXEL_AVERAGE;
    m_voxelIndices = voxelIndicesIJK;
}



/**
 * Restore the state of an instance of a class.
 *
 * @param sceneAttributes
 *    Attributes for the scene.  Scenes may be of different types
 *    (full, generic, etc) and the attributes should be checked when
 *    restoring the scene.
 *
 * @param sceneClass
 *     sceneClass for the instance of a class that implements
 *     this interface.  May be NULL for some types of scenes.
 */
void
ConnectivityDataLoaded::restoreFromScene(const SceneAttributes* sceneAttributes,
                                         const SceneClass* sceneClass)
{
    reset();
    
    if (sceneClass == NULL) {
        return;
    }
    
    m_sceneAssistant->restoreMembers(sceneAttributes,
                                     sceneClass);
    
    m_mode = MODE_NONE;
    const AString modeName = sceneClass->getStringValue("m_mode");
    if (modeName == "MODE_NONE") {
        m_mode = MODE_NONE;
    }
    else if (modeName == "MODE_SURFACE_NODE_AVERAGE") {
        m_mode = MODE_SURFACE_NODE_AVERAGE;
    }
    else if (modeName == "MODE_SURFACE_NODE") {
        m_mode = MODE_SURFACE_NODE;
    }
    else if (modeName == "MODE_VOXEL") {
        m_mode = MODE_VOXEL;
    }
    else if (modeName == "MODE_VOXEL_AVERAGE") {
        m_mode = MODE_VOXEL_AVERAGE;
    }
    else {
        sceneAttributes->addToErrorMessage("Unrecognized mode=" + modeName);
        return;
    }
    
    const ScenePrimitiveArray* surfaceNodeIndicesArray = sceneClass->getPrimitiveArray("m_surfaceNodeIndices");
    if (surfaceNodeIndicesArray != NULL) {
        const int32_t numNodeIndices = surfaceNodeIndicesArray->getNumberOfArrayElements();
        m_surfaceNodeIndices.reserve(numNodeIndices);
        for (int32_t i = 0; i < numNodeIndices; i++) {
            m_surfaceNodeIndices.push_back(surfaceNodeIndicesArray->integerValue(i));
        }
    }
        
    const ScenePrimitiveArray* voxelIndicesArray = sceneClass->getPrimitiveArray("m_voxelIndices");
    if (voxelIndicesArray != NULL) {
        const int64_t numIndices = voxelIndicesArray->getNumberOfArrayElements();
        const int64_t numVoxelIndices = numIndices / 3;
        
        for (int64_t i = 0; i < numVoxelIndices; i++) {
            const int64_t i3 = i * 3;
            VoxelIJK voxelIJK(voxelIndicesArray->integerValue(i3),
                              voxelIndicesArray->integerValue(i3 + 1),
                              voxelIndicesArray->integerValue(i3 + 2));
            m_voxelIndices.push_back(voxelIJK);
        }
    }
}

/**
 * Create a scene for an instance of a class.
 *
 * @param sceneAttributes
 *    Attributes for the scene.  Scenes may be of different types
 *    (full, generic, etc) and the attributes should be checked when
 *    saving the scene.
 *
 * @return Pointer to SceneClass object representing the state of
 *    this object.  Under some circumstances a NULL pointer may be
 *    returned.  Caller will take ownership of returned object.
 */
SceneClass*
ConnectivityDataLoaded::saveToScene(const SceneAttributes* sceneAttributes,
                                    const AString& instanceName)
{
    SceneClass* sceneClass = new SceneClass(instanceName,
                                            "ConnectivityDataLoaded",
                                            1);
    
    m_sceneAssistant->saveMembers(sceneAttributes,
                                  sceneClass);
    
    AString modeName = "MODE_NONE";
    switch (m_mode) {
        case MODE_NONE:
            modeName = "MODE_NONE";
            break;
        case MODE_SURFACE_NODE:
            modeName = "MODE_SURFACE_NODE";
            break;
        case MODE_SURFACE_NODE_AVERAGE:
            modeName = "MODE_SURFACE_NODE_AVERAGE";
            break;
        case MODE_VOXEL:
            modeName = "MODE_VOXEL";
            break;
        case MODE_VOXEL_AVERAGE:
            modeName = "MODE_VOXEL_AVERAGE";
            break;
    }
    sceneClass->addString("m_mode",
                          modeName);
    
    if (m_surfaceNodeIndices.empty() == false) {
        sceneClass->addIntegerArray("m_surfaceNodeIndices",
                                    &m_surfaceNodeIndices[0],
                                    m_surfaceNodeIndices.size());
    }
    
    if (m_voxelIndices.empty() == false) {
        const int32_t numVoxels = m_voxelIndices.size();
        std::vector<int32_t> indices;
        for (int32_t i = 0; i < numVoxels; i++) {
            indices.push_back(m_voxelIndices[i].m_ijk[0]);
            indices.push_back(m_voxelIndices[i].m_ijk[1]);
            indices.push_back(m_voxelIndices[i].m_ijk[2]);
        }
        
        sceneClass->addIntegerArray("m_voxelIndices",
                                    &indices[0],
                                    indices.size());
    }
    
    return sceneClass;
}
