#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/aabboximpl.o \
	${OBJECTDIR}/src/annotation3dimpl.o \
	${OBJECTDIR}/src/bitset.o \
	${OBJECTDIR}/src/block.o \
	${OBJECTDIR}/src/buffer.o \
	${OBJECTDIR}/src/builder.o \
	${OBJECTDIR}/src/builders.o \
	${OBJECTDIR}/src/cell.o \
	${OBJECTDIR}/src/color.o \
	${OBJECTDIR}/src/colortable.o \
	${OBJECTDIR}/src/complexmaterialimpl.o \
	${OBJECTDIR}/src/config.o \
	${OBJECTDIR}/src/config_env.o \
	${OBJECTDIR}/src/connector.o \
	${OBJECTDIR}/src/console.o \
	${OBJECTDIR}/src/cube3dimpl.o \
	${OBJECTDIR}/src/curve3dimpl.o \
	${OBJECTDIR}/src/delaunayimpl.o \
	${OBJECTDIR}/src/demblock.o \
	${OBJECTDIR}/src/demdatainfo.o \
	${OBJECTDIR}/src/demdbinfo.o \
	${OBJECTDIR}/src/demgrid.o \
	${OBJECTDIR}/src/demgridimpl.o \
	${OBJECTDIR}/src/deminfo.o \
	${OBJECTDIR}/src/demlayerinfo.o \
	${OBJECTDIR}/src/describablebuffer.o \
	${OBJECTDIR}/src/domblock.o \
	${OBJECTDIR}/src/domdatainfo.o \
	${OBJECTDIR}/src/domdbinfo.o \
	${OBJECTDIR}/src/domgrid.o \
	${OBJECTDIR}/src/domgridimpl.o \
	${OBJECTDIR}/src/dominfo.o \
	${OBJECTDIR}/src/domlayerinfo.o \
	${OBJECTDIR}/src/eclipsemeshimpl.o \
	${OBJECTDIR}/src/eclipsemeshreader.o \
	${OBJECTDIR}/src/envelope2d.o \
	${OBJECTDIR}/src/envelope3d.o \
	${OBJECTDIR}/src/factory.o \
	${OBJECTDIR}/src/feature.o \
	${OBJECTDIR}/src/featureclass.o \
	${OBJECTDIR}/src/featureclassimpl.o \
	${OBJECTDIR}/src/featureimpl.o \
	${OBJECTDIR}/src/featureset.o \
	${OBJECTDIR}/src/featuresetimpl.o \
	${OBJECTDIR}/src/frustum.o \
	${OBJECTDIR}/src/frustumimpl.o \
	${OBJECTDIR}/src/geometry.o \
	${OBJECTDIR}/src/geometry2d.o \
	${OBJECTDIR}/src/geometry3dimpl.o \
	${OBJECTDIR}/src/geometryfactory.o \
	${OBJECTDIR}/src/geometryimpl.o \
	${OBJECTDIR}/src/globalfuns.o \
	${OBJECTDIR}/src/group3dimpl.o \
	${OBJECTDIR}/src/identifier.o \
	${OBJECTDIR}/src/image.o \
	${OBJECTDIR}/src/indexbuffer.o \
	${OBJECTDIR}/src/layer.o \
	${OBJECTDIR}/src/layerimpl.o \
	${OBJECTDIR}/src/linearring3dimpl.o \
	${OBJECTDIR}/src/linestring3dimpl.o \
	${OBJECTDIR}/src/lock.o \
	${OBJECTDIR}/src/lodinfo.o \
	${OBJECTDIR}/src/lodinfoimpl.o \
	${OBJECTDIR}/src/material.o \
	${OBJECTDIR}/src/materialimpl.o \
	${OBJECTDIR}/src/mathex.o \
	${OBJECTDIR}/src/matrix.o \
	${OBJECTDIR}/src/matrix3x3.o \
	${OBJECTDIR}/src/matrix4x4.o \
	${OBJECTDIR}/src/mesh.o \
	${OBJECTDIR}/src/multipoint3dimpl.o \
	${OBJECTDIR}/src/multipolygon3dimpl.o \
	${OBJECTDIR}/src/multipolyline3dimpl.o \
	${OBJECTDIR}/src/multisolid3dimpl.o \
	${OBJECTDIR}/src/obboximpl.o \
	${OBJECTDIR}/src/parameterizedcurve3dimpl.o \
	${OBJECTDIR}/src/parameterizedsurface3dimpl.o \
	${OBJECTDIR}/src/point3dimpl.o \
	${OBJECTDIR}/src/polygon3dimpl.o \
	${OBJECTDIR}/src/prism3dimpl.o \
	${OBJECTDIR}/src/project.o \
	${OBJECTDIR}/src/projectimpl.o \
	${OBJECTDIR}/src/property.o \
	${OBJECTDIR}/src/proxy.o \
	${OBJECTDIR}/src/quaternion.o \
	${OBJECTDIR}/src/ray.o \
	${OBJECTDIR}/src/rayimpl.o \
	${OBJECTDIR}/src/rectangle3dimpl.o \
	${OBJECTDIR}/src/reference3dimpl.o \
	${OBJECTDIR}/src/render.o \
	${OBJECTDIR}/src/selector.o \
	${OBJECTDIR}/src/semanticactiontopoinfoimpl.o \
	${OBJECTDIR}/src/semanticgeometrytopoinfoimpl.o \
	${OBJECTDIR}/src/shadedvertex3dvisitorimpl.o \
	${OBJECTDIR}/src/shadedvertex3fvisitorimpl.o \
	${OBJECTDIR}/src/sharedmodel.o \
	${OBJECTDIR}/src/sharedmodelimpl.o \
	${OBJECTDIR}/src/simplematerialimpl.o \
	${OBJECTDIR}/src/singlepoint3dimpl.o \
	${OBJECTDIR}/src/solid3dimpl.o \
	${OBJECTDIR}/src/sourcedatainfo.o \
	${OBJECTDIR}/src/spatialfuns.o \
	${OBJECTDIR}/src/sphere3d.o \
	${OBJECTDIR}/src/surface3dimpl.o \
	${OBJECTDIR}/src/temporaryset.o \
	${OBJECTDIR}/src/terrain.o \
	${OBJECTDIR}/src/tetrahedron3dimpl.o \
	${OBJECTDIR}/src/texture.o \
	${OBJECTDIR}/src/texturedvertex3dvisitorimpl.o \
	${OBJECTDIR}/src/texturedvertex3fvisitorimpl.o \
	${OBJECTDIR}/src/textureimpl.o \
	${OBJECTDIR}/src/topoinfo.o \
	${OBJECTDIR}/src/topoinfoimpl.o \
	${OBJECTDIR}/src/topoitem.o \
	${OBJECTDIR}/src/topoitemimpl.o \
	${OBJECTDIR}/src/topology.o \
	${OBJECTDIR}/src/triangle3dimpl.o \
	${OBJECTDIR}/src/trianglemesh3dimpl.o \
	${OBJECTDIR}/src/tritool.o \
	${OBJECTDIR}/src/value.o \
	${OBJECTDIR}/src/valutils.o \
	${OBJECTDIR}/src/versatilevertex3dvisitorimpl.o \
	${OBJECTDIR}/src/versatilevertex3fvisitorimpl.o \
	${OBJECTDIR}/src/vertex2d.o \
	${OBJECTDIR}/src/vertex2f.o \
	${OBJECTDIR}/src/vertex3d.o \
	${OBJECTDIR}/src/vertex3dvisitorimpl.o \
	${OBJECTDIR}/src/vertex3f.o \
	${OBJECTDIR}/src/vertex3fvisitorimpl.o \
	${OBJECTDIR}/src/vertexbuffer.o \
	${OBJECTDIR}/src/vertexcollection2d.o \
	${OBJECTDIR}/src/vertexvisitor.o \
	${OBJECTDIR}/src/volume3dimpl.o \
	${OBJECTDIR}/src/voxel3dimpl.o \
	${OBJECTDIR}/src/workspace.o \
	${OBJECTDIR}/src/workspaceimpl.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11
CXXFLAGS=-std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/src/aabboximpl.o: src/aabboximpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/aabboximpl.o src/aabboximpl.cpp

${OBJECTDIR}/src/annotation3dimpl.o: src/annotation3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/annotation3dimpl.o src/annotation3dimpl.cpp

${OBJECTDIR}/src/bitset.o: src/bitset.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/bitset.o src/bitset.cpp

${OBJECTDIR}/src/block.o: src/block.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/block.o src/block.cpp

${OBJECTDIR}/src/buffer.o: src/buffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/buffer.o src/buffer.cpp

${OBJECTDIR}/src/builder.o: src/builder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/builder.o src/builder.cpp

${OBJECTDIR}/src/builders.o: src/builders.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/builders.o src/builders.cpp

${OBJECTDIR}/src/cell.o: src/cell.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cell.o src/cell.cpp

${OBJECTDIR}/src/color.o: src/color.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/color.o src/color.cpp

${OBJECTDIR}/src/colortable.o: src/colortable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/colortable.o src/colortable.cpp

${OBJECTDIR}/src/complexmaterialimpl.o: src/complexmaterialimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/complexmaterialimpl.o src/complexmaterialimpl.cpp

${OBJECTDIR}/src/config.o: src/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/config.o src/config.cpp

${OBJECTDIR}/src/config_env.o: src/config_env.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/config_env.o src/config_env.cpp

${OBJECTDIR}/src/connector.o: src/connector.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/connector.o src/connector.cpp

${OBJECTDIR}/src/console.o: src/console.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/console.o src/console.cpp

${OBJECTDIR}/src/cube3dimpl.o: src/cube3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/cube3dimpl.o src/cube3dimpl.cpp

${OBJECTDIR}/src/curve3dimpl.o: src/curve3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/curve3dimpl.o src/curve3dimpl.cpp

${OBJECTDIR}/src/delaunayimpl.o: src/delaunayimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/delaunayimpl.o src/delaunayimpl.cpp

${OBJECTDIR}/src/demblock.o: src/demblock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/demblock.o src/demblock.cpp

${OBJECTDIR}/src/demdatainfo.o: src/demdatainfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/demdatainfo.o src/demdatainfo.cpp

${OBJECTDIR}/src/demdbinfo.o: src/demdbinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/demdbinfo.o src/demdbinfo.cpp

${OBJECTDIR}/src/demgrid.o: src/demgrid.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/demgrid.o src/demgrid.cpp

${OBJECTDIR}/src/demgridimpl.o: src/demgridimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/demgridimpl.o src/demgridimpl.cpp

${OBJECTDIR}/src/deminfo.o: src/deminfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/deminfo.o src/deminfo.cpp

${OBJECTDIR}/src/demlayerinfo.o: src/demlayerinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/demlayerinfo.o src/demlayerinfo.cpp

${OBJECTDIR}/src/describablebuffer.o: src/describablebuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/describablebuffer.o src/describablebuffer.cpp

${OBJECTDIR}/src/domblock.o: src/domblock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/domblock.o src/domblock.cpp

${OBJECTDIR}/src/domdatainfo.o: src/domdatainfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/domdatainfo.o src/domdatainfo.cpp

${OBJECTDIR}/src/domdbinfo.o: src/domdbinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/domdbinfo.o src/domdbinfo.cpp

${OBJECTDIR}/src/domgrid.o: src/domgrid.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/domgrid.o src/domgrid.cpp

${OBJECTDIR}/src/domgridimpl.o: src/domgridimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/domgridimpl.o src/domgridimpl.cpp

${OBJECTDIR}/src/dominfo.o: src/dominfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dominfo.o src/dominfo.cpp

${OBJECTDIR}/src/domlayerinfo.o: src/domlayerinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/domlayerinfo.o src/domlayerinfo.cpp

${OBJECTDIR}/src/eclipsemeshimpl.o: src/eclipsemeshimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/eclipsemeshimpl.o src/eclipsemeshimpl.cpp

${OBJECTDIR}/src/eclipsemeshreader.o: src/eclipsemeshreader.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/eclipsemeshreader.o src/eclipsemeshreader.cpp

${OBJECTDIR}/src/envelope2d.o: src/envelope2d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/envelope2d.o src/envelope2d.cpp

${OBJECTDIR}/src/envelope3d.o: src/envelope3d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/envelope3d.o src/envelope3d.cpp

${OBJECTDIR}/src/factory.o: src/factory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/factory.o src/factory.cpp

${OBJECTDIR}/src/feature.o: src/feature.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/feature.o src/feature.cpp

${OBJECTDIR}/src/featureclass.o: src/featureclass.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/featureclass.o src/featureclass.cpp

${OBJECTDIR}/src/featureclassimpl.o: src/featureclassimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/featureclassimpl.o src/featureclassimpl.cpp

${OBJECTDIR}/src/featureimpl.o: src/featureimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/featureimpl.o src/featureimpl.cpp

${OBJECTDIR}/src/featureset.o: src/featureset.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/featureset.o src/featureset.cpp

${OBJECTDIR}/src/featuresetimpl.o: src/featuresetimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/featuresetimpl.o src/featuresetimpl.cpp

${OBJECTDIR}/src/frustum.o: src/frustum.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frustum.o src/frustum.cpp

${OBJECTDIR}/src/frustumimpl.o: src/frustumimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/frustumimpl.o src/frustumimpl.cpp

${OBJECTDIR}/src/geometry.o: src/geometry.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometry.o src/geometry.cpp

${OBJECTDIR}/src/geometry2d.o: src/geometry2d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometry2d.o src/geometry2d.cpp

${OBJECTDIR}/src/geometry3dimpl.o: src/geometry3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometry3dimpl.o src/geometry3dimpl.cpp

${OBJECTDIR}/src/geometryfactory.o: src/geometryfactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometryfactory.o src/geometryfactory.cpp

${OBJECTDIR}/src/geometryimpl.o: src/geometryimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/geometryimpl.o src/geometryimpl.cpp

${OBJECTDIR}/src/globalfuns.o: src/globalfuns.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/globalfuns.o src/globalfuns.cpp

${OBJECTDIR}/src/group3dimpl.o: src/group3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/group3dimpl.o src/group3dimpl.cpp

${OBJECTDIR}/src/identifier.o: src/identifier.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/identifier.o src/identifier.cpp

${OBJECTDIR}/src/image.o: src/image.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/image.o src/image.cpp

${OBJECTDIR}/src/indexbuffer.o: src/indexbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/indexbuffer.o src/indexbuffer.cpp

${OBJECTDIR}/src/layer.o: src/layer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/layer.o src/layer.cpp

${OBJECTDIR}/src/layerimpl.o: src/layerimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/layerimpl.o src/layerimpl.cpp

${OBJECTDIR}/src/linearring3dimpl.o: src/linearring3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/linearring3dimpl.o src/linearring3dimpl.cpp

${OBJECTDIR}/src/linestring3dimpl.o: src/linestring3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/linestring3dimpl.o src/linestring3dimpl.cpp

${OBJECTDIR}/src/lock.o: src/lock.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lock.o src/lock.cpp

${OBJECTDIR}/src/lodinfo.o: src/lodinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lodinfo.o src/lodinfo.cpp

${OBJECTDIR}/src/lodinfoimpl.o: src/lodinfoimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lodinfoimpl.o src/lodinfoimpl.cpp

${OBJECTDIR}/src/material.o: src/material.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/material.o src/material.cpp

${OBJECTDIR}/src/materialimpl.o: src/materialimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/materialimpl.o src/materialimpl.cpp

${OBJECTDIR}/src/mathex.o: src/mathex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mathex.o src/mathex.cpp

${OBJECTDIR}/src/matrix.o: src/matrix.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/matrix.o src/matrix.cpp

${OBJECTDIR}/src/matrix3x3.o: src/matrix3x3.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/matrix3x3.o src/matrix3x3.cpp

${OBJECTDIR}/src/matrix4x4.o: src/matrix4x4.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/matrix4x4.o src/matrix4x4.cpp

${OBJECTDIR}/src/mesh.o: src/mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/mesh.o src/mesh.cpp

${OBJECTDIR}/src/multipoint3dimpl.o: src/multipoint3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/multipoint3dimpl.o src/multipoint3dimpl.cpp

${OBJECTDIR}/src/multipolygon3dimpl.o: src/multipolygon3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/multipolygon3dimpl.o src/multipolygon3dimpl.cpp

${OBJECTDIR}/src/multipolyline3dimpl.o: src/multipolyline3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/multipolyline3dimpl.o src/multipolyline3dimpl.cpp

${OBJECTDIR}/src/multisolid3dimpl.o: src/multisolid3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/multisolid3dimpl.o src/multisolid3dimpl.cpp

${OBJECTDIR}/src/obboximpl.o: src/obboximpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/obboximpl.o src/obboximpl.cpp

${OBJECTDIR}/src/parameterizedcurve3dimpl.o: src/parameterizedcurve3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/parameterizedcurve3dimpl.o src/parameterizedcurve3dimpl.cpp

${OBJECTDIR}/src/parameterizedsurface3dimpl.o: src/parameterizedsurface3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/parameterizedsurface3dimpl.o src/parameterizedsurface3dimpl.cpp

${OBJECTDIR}/src/point3dimpl.o: src/point3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/point3dimpl.o src/point3dimpl.cpp

${OBJECTDIR}/src/polygon3dimpl.o: src/polygon3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/polygon3dimpl.o src/polygon3dimpl.cpp

${OBJECTDIR}/src/prism3dimpl.o: src/prism3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/prism3dimpl.o src/prism3dimpl.cpp

${OBJECTDIR}/src/project.o: src/project.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/project.o src/project.cpp

${OBJECTDIR}/src/projectimpl.o: src/projectimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/projectimpl.o src/projectimpl.cpp

${OBJECTDIR}/src/property.o: src/property.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/property.o src/property.cpp

${OBJECTDIR}/src/proxy.o: src/proxy.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/proxy.o src/proxy.cpp

${OBJECTDIR}/src/quaternion.o: src/quaternion.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/quaternion.o src/quaternion.cpp

${OBJECTDIR}/src/ray.o: src/ray.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/ray.o src/ray.cpp

${OBJECTDIR}/src/rayimpl.o: src/rayimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rayimpl.o src/rayimpl.cpp

${OBJECTDIR}/src/rectangle3dimpl.o: src/rectangle3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/rectangle3dimpl.o src/rectangle3dimpl.cpp

${OBJECTDIR}/src/reference3dimpl.o: src/reference3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/reference3dimpl.o src/reference3dimpl.cpp

${OBJECTDIR}/src/render.o: src/render.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/render.o src/render.cpp

${OBJECTDIR}/src/selector.o: src/selector.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/selector.o src/selector.cpp

${OBJECTDIR}/src/semanticactiontopoinfoimpl.o: src/semanticactiontopoinfoimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/semanticactiontopoinfoimpl.o src/semanticactiontopoinfoimpl.cpp

${OBJECTDIR}/src/semanticgeometrytopoinfoimpl.o: src/semanticgeometrytopoinfoimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/semanticgeometrytopoinfoimpl.o src/semanticgeometrytopoinfoimpl.cpp

${OBJECTDIR}/src/shadedvertex3dvisitorimpl.o: src/shadedvertex3dvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shadedvertex3dvisitorimpl.o src/shadedvertex3dvisitorimpl.cpp

${OBJECTDIR}/src/shadedvertex3fvisitorimpl.o: src/shadedvertex3fvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/shadedvertex3fvisitorimpl.o src/shadedvertex3fvisitorimpl.cpp

${OBJECTDIR}/src/sharedmodel.o: src/sharedmodel.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sharedmodel.o src/sharedmodel.cpp

${OBJECTDIR}/src/sharedmodelimpl.o: src/sharedmodelimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sharedmodelimpl.o src/sharedmodelimpl.cpp

${OBJECTDIR}/src/simplematerialimpl.o: src/simplematerialimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/simplematerialimpl.o src/simplematerialimpl.cpp

${OBJECTDIR}/src/singlepoint3dimpl.o: src/singlepoint3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/singlepoint3dimpl.o src/singlepoint3dimpl.cpp

${OBJECTDIR}/src/solid3dimpl.o: src/solid3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/solid3dimpl.o src/solid3dimpl.cpp

${OBJECTDIR}/src/sourcedatainfo.o: src/sourcedatainfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sourcedatainfo.o src/sourcedatainfo.cpp

${OBJECTDIR}/src/spatialfuns.o: src/spatialfuns.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/spatialfuns.o src/spatialfuns.cpp

${OBJECTDIR}/src/sphere3d.o: src/sphere3d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/sphere3d.o src/sphere3d.cpp

${OBJECTDIR}/src/surface3dimpl.o: src/surface3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/surface3dimpl.o src/surface3dimpl.cpp

${OBJECTDIR}/src/temporaryset.o: src/temporaryset.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/temporaryset.o src/temporaryset.cpp

${OBJECTDIR}/src/terrain.o: src/terrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/terrain.o src/terrain.cpp

${OBJECTDIR}/src/tetrahedron3dimpl.o: src/tetrahedron3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tetrahedron3dimpl.o src/tetrahedron3dimpl.cpp

${OBJECTDIR}/src/texture.o: src/texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/texture.o src/texture.cpp

${OBJECTDIR}/src/texturedvertex3dvisitorimpl.o: src/texturedvertex3dvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/texturedvertex3dvisitorimpl.o src/texturedvertex3dvisitorimpl.cpp

${OBJECTDIR}/src/texturedvertex3fvisitorimpl.o: src/texturedvertex3fvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/texturedvertex3fvisitorimpl.o src/texturedvertex3fvisitorimpl.cpp

${OBJECTDIR}/src/textureimpl.o: src/textureimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/textureimpl.o src/textureimpl.cpp

${OBJECTDIR}/src/topoinfo.o: src/topoinfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/topoinfo.o src/topoinfo.cpp

${OBJECTDIR}/src/topoinfoimpl.o: src/topoinfoimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/topoinfoimpl.o src/topoinfoimpl.cpp

${OBJECTDIR}/src/topoitem.o: src/topoitem.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/topoitem.o src/topoitem.cpp

${OBJECTDIR}/src/topoitemimpl.o: src/topoitemimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/topoitemimpl.o src/topoitemimpl.cpp

${OBJECTDIR}/src/topology.o: src/topology.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/topology.o src/topology.cpp

${OBJECTDIR}/src/triangle3dimpl.o: src/triangle3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/triangle3dimpl.o src/triangle3dimpl.cpp

${OBJECTDIR}/src/trianglemesh3dimpl.o: src/trianglemesh3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/trianglemesh3dimpl.o src/trianglemesh3dimpl.cpp

${OBJECTDIR}/src/tritool.o: src/tritool.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/tritool.o src/tritool.cpp

${OBJECTDIR}/src/value.o: src/value.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/value.o src/value.cpp

${OBJECTDIR}/src/valutils.o: src/valutils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/valutils.o src/valutils.cpp

${OBJECTDIR}/src/versatilevertex3dvisitorimpl.o: src/versatilevertex3dvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/versatilevertex3dvisitorimpl.o src/versatilevertex3dvisitorimpl.cpp

${OBJECTDIR}/src/versatilevertex3fvisitorimpl.o: src/versatilevertex3fvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/versatilevertex3fvisitorimpl.o src/versatilevertex3fvisitorimpl.cpp

${OBJECTDIR}/src/vertex2d.o: src/vertex2d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertex2d.o src/vertex2d.cpp

${OBJECTDIR}/src/vertex2f.o: src/vertex2f.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertex2f.o src/vertex2f.cpp

${OBJECTDIR}/src/vertex3d.o: src/vertex3d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertex3d.o src/vertex3d.cpp

${OBJECTDIR}/src/vertex3dvisitorimpl.o: src/vertex3dvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertex3dvisitorimpl.o src/vertex3dvisitorimpl.cpp

${OBJECTDIR}/src/vertex3f.o: src/vertex3f.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertex3f.o src/vertex3f.cpp

${OBJECTDIR}/src/vertex3fvisitorimpl.o: src/vertex3fvisitorimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertex3fvisitorimpl.o src/vertex3fvisitorimpl.cpp

${OBJECTDIR}/src/vertexbuffer.o: src/vertexbuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertexbuffer.o src/vertexbuffer.cpp

${OBJECTDIR}/src/vertexcollection2d.o: src/vertexcollection2d.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertexcollection2d.o src/vertexcollection2d.cpp

${OBJECTDIR}/src/vertexvisitor.o: src/vertexvisitor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/vertexvisitor.o src/vertexvisitor.cpp

${OBJECTDIR}/src/volume3dimpl.o: src/volume3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/volume3dimpl.o src/volume3dimpl.cpp

${OBJECTDIR}/src/voxel3dimpl.o: src/voxel3dimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/voxel3dimpl.o src/voxel3dimpl.cpp

${OBJECTDIR}/src/workspace.o: src/workspace.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/workspace.o src/workspace.cpp

${OBJECTDIR}/src/workspaceimpl.o: src/workspaceimpl.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -DCORE_EXPORTS -Iinc -fPIC  -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/workspaceimpl.o src/workspaceimpl.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libcore.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
