#pragma once
#include <array>
#include <cmath>

struct Matrix4 {
    std::array<double,16> m{};
    Matrix4() noexcept { identity(); }
    void identity() noexcept {
        m = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    }
    static Matrix4 multiply(const Matrix4& a, const Matrix4& b) noexcept {
        Matrix4 r;
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j){
                double s=0;
                for(int k=0;k<4;++k) s+=a.m[i*4+k]*b.m[k*4+j];
                r.m[i*4+j]=s;
            }
        return r;
    }
};
