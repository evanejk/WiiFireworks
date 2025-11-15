#ifndef VECTOR_H
#define VECTOR_H


class Vector3f
{
    public:
        Vector3f(float x, float y, float z);
        float x;
        float y;
        float z;
        void normalize();

    protected:

    private:
};

#endif // VECTOR_H
