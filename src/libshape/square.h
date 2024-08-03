#ifndef SQUARE_H
#define SQUARE_H

#include "ishape.h" // 确保包含路径正确
namespace sdfibm {

class Square : public IShape, _shapecreator<Square> {
private:
    scalar m_sideLength;
    scalar m_sideLengthSQR;

public:
    Square(const dictionary& para) {
        m_sideLength = Foam::readScalar(para.lookup("sideLength"));
        m_com = para.lookupOrDefault("com", vector::zero);

        m_sideLengthSQR = m_sideLength * m_sideLength;

        // 在二维中，'volume' 代表面积
        m_volume = m_sideLengthSQR;
        m_volumeINV = 1.0 / m_volume;

        // 对于二维形状，惯性矩阵的计算方式略有不同，这里仅作示例
        scalar tmp = 1.0/6.0 * m_sideLengthSQR;
        m_moi[0] = tmp; m_moi[4] = tmp; m_moi[8] = tmp;

        m_moiINV = Foam::inv(m_moi);
        m_radiusB = m_sideLength / std::sqrt(2.0); // 正方形对角线的一半作为包围球半径
    }

    inline scalar getSideLength() const { return m_sideLength; }
    inline scalar getVolume() const { return m_volume; } // 这里实际上返回的是面积

    SHAPETYPENAME("Square")
    virtual std::string description() const override {
        return "square, side length = " + std::to_string(m_sideLength);
    }

    virtual inline bool isInside(const vector& p) const override {
        vector relPos = p - m_com; // 相对于中心的位置
        return std::abs(relPos.x()) <= m_sideLength / 2.0 && std::abs(relPos.y()) <= m_sideLength / 2.0;
    }

    virtual inline scalar signedDistance(const vector& p) const override {
        vector relPos = p - m_com; // 相对于中心的位置
        scalar dx = std::max(std::abs(relPos.x()) - m_sideLength / 2.0, 0.0);
        scalar dy = std::max(std::abs(relPos.y()) - m_sideLength / 2.0, 0.0);
        return std::sqrt(dx*dx + dy*dy);
    }
};

} // namespace sdfibm
#endif

