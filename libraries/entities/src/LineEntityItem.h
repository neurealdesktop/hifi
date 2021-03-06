//
//  LineEntityItem.h
//  libraries/entities/src
//
//  Created by Seth Alves on 5/11/15.
//  Copyright 2015 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_LineEntityItem_h
#define hifi_LineEntityItem_h

#include "EntityItem.h"

class LineEntityItem : public EntityItem {
 public:
    static EntityItemPointer factory(const EntityItemID& entityID, const EntityItemProperties& properties);

    LineEntityItem(const EntityItemID& entityItemID);

    ALLOW_INSTANTIATION // This class can be instantiated

    // methods for getting/setting all properties of an entity
    virtual EntityItemProperties getProperties(const EntityPropertyFlags& desiredProperties, bool allowEmptyDesiredProperties) const override;
    virtual bool setProperties(const EntityItemProperties& properties) override;

    virtual EntityPropertyFlags getEntityProperties(EncodeBitstreamParams& params) const override;

    virtual void appendSubclassData(OctreePacketData* packetData, EncodeBitstreamParams& params,
                                    EntityTreeElementExtraEncodeDataPointer modelTreeElementExtraEncodeData,
                                    EntityPropertyFlags& requestedProperties,
                                    EntityPropertyFlags& propertyFlags,
                                    EntityPropertyFlags& propertiesDidntFit,
                                    int& propertyCount,
                                    OctreeElement::AppendState& appendState) const override;

    virtual int readEntitySubclassDataFromBuffer(const unsigned char* data, int bytesLeftToRead,
                                                 ReadBitstreamToTreeParams& args,
                                                 EntityPropertyFlags& propertyFlags, bool overwriteLocalData,
                                                 bool& somethingChanged) override;

    glm::u8vec3 getColor() const;
    void setColor(const glm::u8vec3& value);

    void setLineWidth(float lineWidth);
    float getLineWidth() const;

    bool setLinePoints(const QVector<glm::vec3>& points);
    bool appendPoint(const glm::vec3& point);

    QVector<glm::vec3> getLinePoints() const;

    virtual ShapeType getShapeType() const override { return SHAPE_TYPE_NONE; }

    // never have a ray intersection pick a LineEntityItem.
    virtual bool supportsDetailedIntersection() const override { return true; }
    virtual bool findDetailedRayIntersection(const glm::vec3& origin, const glm::vec3& direction,
                                             OctreeElementPointer& element, float& distance,
                                             BoxFace& face, glm::vec3& surfaceNormal,
                                             QVariantMap& extraInfo,
                                             bool precisionPicking) const override { return false; }
    virtual bool findDetailedParabolaIntersection(const glm::vec3& origin, const glm::vec3& velocity,
                                                  const glm::vec3& acceleration, OctreeElementPointer& element, float& parabolicDistance,
                                                  BoxFace& face, glm::vec3& surfaceNormal,
                                                  QVariantMap& extraInfo,
                                                  bool precisionPicking) const override { return false; }
    bool pointsChanged() const { return _pointsChanged; }
    void resetPointsChanged();
    virtual void debugDump() const override;
    static const float DEFAULT_LINE_WIDTH;
    static const int MAX_POINTS_PER_LINE;

 private:
    glm::u8vec3 _color;
    float _lineWidth { DEFAULT_LINE_WIDTH };
    QVector<glm::vec3> _points;
    bool _pointsChanged { true };
};

#endif // hifi_LineEntityItem_h
