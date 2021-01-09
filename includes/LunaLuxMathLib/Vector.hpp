//
// Created by luket on 31/12/2020.
//
#ifndef LUNALUXMATHLIB_VECTOR_HPP
#define LUNALUXMATHLIB_VECTOR_HPP
//TODO: write documentation
namespace LunaLuxMathLib
{
    template<typename type>
    class [[maybe_unused]] Vector2
    {
    private:
        type x{ 0 }, y{ 0 };
    public:
        [[maybe_unused]] Vector2(type in_x,type in_y)
        {
            this->x = in_x;
            this->y = in_y;
        }

        [[maybe_unused]] Vector2(type* in_x,type* in_y)
        {
            this->x = *in_x;
            this->y = *in_y;
        }

        [[maybe_unused]] explicit Vector2(Vector2<type>* in)
        {
            this->x = in->getX();
            this->y = in->getY();
        }

        type getX() {return this->x;}

        type getY() {return this->y;}

        [[maybe_unused]] bool Not_Equl_X(Vector2<type> in) {return (this->getX() != in.getX());}

        [[maybe_unused]] bool Not_Equl_Y(Vector2<type> in) {return (this->getY() != in.getY());}

        [[maybe_unused]] bool Equl_X(Vector2<type> in) {return (this->getX() == in.getX());}

        [[maybe_unused]] bool Equl_Y(Vector2<type> in) {return (this->getY() == in.getY());}

        [[maybe_unused]] bool Equl_GreaterThan_X(Vector2<type> in) {return (this->getX() >= in.getX());}

        [[maybe_unused]] bool Equl_GreaterThan_Y(Vector2<type> in) {return (this->getY() >= in.getY());}

        [[maybe_unused]] bool Equl_LesserThan_X(Vector2<type> in) {return (this->getX() <= in.getX());}

        [[maybe_unused]] bool Equl_LesserThan_Y(Vector2<type> in) {return (this->getY() <= in.getY());}

        [[maybe_unused]] bool GreaterThan_X(Vector2<type> in) {return (this->getX() > in.getX());}

        [[maybe_unused]] bool GreaterThan_Y(Vector2<type> in) {return (this->getY() > in.getY());}

        [[maybe_unused]] bool LesserThan_X(Vector2<type> in) {return (this->getX() < in.getX());}

        [[maybe_unused]] bool LesserThan_Y(Vector2<type> in) {return (this->getY() < in.getY());}

        [[maybe_unused]] void Add_X(Vector2<type> in) {this->x = this->getX() + in.getX();}

        [[maybe_unused]] void Add_Y(Vector2<type> in) {this->y = this->getY() + in.getY();}
        
        [[maybe_unused]] void Sub_X(Vector2<type> in) {this->x = this->getX() - in.getX();}

        [[maybe_unused]] void Sub_Y(Vector2<type> in) {this->y = this->getY() - in.getY();}

        [[maybe_unused]] void Div_X(Vector2<type> in) {this->x = this->getX() / in.getX();}

        [[maybe_unused]] void Div_Y(Vector2<type> in) {this->y = this->getY() / in.getY();}

        [[maybe_unused]] void Mul_X(Vector2<type> in) {this->x = this->getX() * in.getX();}

        [[maybe_unused]] void Mul_Y(Vector2<type> in) {this->y = this->getY() * in.getY();}

        [[maybe_unused]] bool Not_Equl_X(type in) {return (this->getX() != in);}

        [[maybe_unused]] bool Not_Equl_Y(type in) {return (this->getY() != in);}

        [[maybe_unused]] bool Equl_X(type in) {return (this->getX() == in);}

        [[maybe_unused]] bool Equl_Y(type in) {return (this->getY() == in);}

        [[maybe_unused]] bool Equl_GreaterThan_X(type in) {return (this->getX() >= in);}

        [[maybe_unused]] bool Equl_GreaterThan_Y(type in) {return (this->getY() >= in);}

        [[maybe_unused]] bool Equl_LesserThan_X(type in) {return (this->getX() <= in);}

        [[maybe_unused]] bool Equl_LesserThan_Y(type in) {return (this->getY() <= in);}

        [[maybe_unused]] bool GreaterThan_X(type in) {return (this->getX() > in);}

        [[maybe_unused]] bool GreaterThan_Y(type in) {return (this->getY() > in);}

        [[maybe_unused]] bool LesserThan_X(type in) {return (this->getX() < in);}

        [[maybe_unused]] bool LesserThan_Y(type in) {return (this->getY() < in);}

        [[maybe_unused]] void Add_X(type in) {this->x = this->getX() + in;}

        [[maybe_unused]] void Add_Y(type in) {this->y = this->getY() + in;}

        [[maybe_unused]] void Sub_X(type in) {this->x = this->getX() - in;}

        [[maybe_unused]] void Sub_Y(type in) {this->y = this->getY() - in;}

        [[maybe_unused]] void Div_X(type in) {this->x = this->getX() / in;}

        [[maybe_unused]] void Div_Y(type in) {this->y = this->getY() / in;}

        [[maybe_unused]] void Mul_X(type in) {this->x = this->getX() * in;}

        [[maybe_unused]] void Mul_Y(type in) {this->y = this->getY() * in;}


        Vector2<type> operator +(Vector2<type> in)
        {
            this->x = (this->getX() + in.getX());
            this->y = (this->getY() + in.getY());
            return *this;
        }

        Vector2<type> operator *(Vector2<type> in)
        {
            this->x = (this->getX() * in.getX());
            this->y = (this->getY() * in.getY());
            return *this;
        }

        Vector2<type> operator -(Vector2<type> in)
        {
            this->x = (this->getX() - in.getX());
            this->y = (this->getY() - in.getY());
            return *this;
        }

        Vector2<type> operator /(Vector2<type> in)
        {
            this->x = (this->getX() / in.getX());
            this->y = (this->getY() / in.getY());
            return *this;
        }
        
        bool operator <(Vector2<type> in){return ((this->getX() < in.getX()) & (this->getY() < in.getY()));}

        bool operator >(Vector2<type> in){return ((this->getX() > in.getX()) & (this->getY() > in.getY()));}

        Vector2<type> operator +=(Vector2<type> in)
        {
            this->x = (this->getX() += in.getX());
            this->y = (this->getY() += in.getY());
            return *this;
        }

        Vector2<type> operator *=(Vector2<type> in)
        {
            this->x = (this->getX() *= in.getX());
            this->y = (this->getY() *= in.getY());
            return *this;
        }

        Vector2<type> operator -=(Vector2<type> in)
        {
            this->x = (this->getX() -= in.getX());
            this->y = (this->getY() -= in.getY());
            return *this;
        }

        Vector2<type> operator /=(Vector2<type> in)
        {
            this->x = (this->getX() /= in.getX());
            this->y = (this->getY() /= in.getY());
            return *this;
        }

        bool operator <=(Vector2<type> in)
        {return ((this->getX() <= in.getX()) & (this->getY() <= in.getY()));}

        bool operator >=(Vector2<type> in)
        {return ((this->getX() >= in.getX()) & (this->getY() >= in.getY()));}

        bool operator ==(Vector2<type> in)
        {return ((this->getX() == in.getX()) & (this->getY() == in.getY()));}

        bool operator !=(Vector2<type> in)
        {return ((this->getX() != in.getX()) & (this->getY() != in.getY()));}
    };

    template<typename type>
    class [[maybe_unused]] Vector3
    {
    private:
        type x{ 0 }, y{ 0 }, z{ 0 };
    public:
        [[maybe_unused]] Vector3(type in_x,type in_y,type in_z)
        {
            this->x = in_x;
            this->y = in_y;
            this->z = in_z;
        }

        [[maybe_unused]] Vector3(type* in_x,type* in_y,type* in_z)
        {
            this->x = *in_x;
            this->y = *in_y;
            this->z = *in_z;
        }

        [[maybe_unused]] explicit Vector3(Vector3<type>* in)
        {
            this->x = in->getX();
            this->y = in->getY();
            this->z = in->getZ();
        }

        type getX() {return this->x;}

        type getY() {return this->y;}

        type getZ() {return this->z;}

        [[maybe_unused]] bool Not_Equl_X(Vector3<type> in)
        {return (this->getX() != in.getX());}

        [[maybe_unused]] bool Not_Equl_Y(Vector3<type> in)
        {return (this->getY() != in.getY());}

        [[maybe_unused]] bool Not_Equl_Z(Vector3<type> in)
        {return (this->getZ() != in.getZ());}

        [[maybe_unused]] bool Equl_X(Vector3<type> in)
        {return (this->getX() == in.getX());}

        [[maybe_unused]] bool Equl_Y(Vector3<type> in)
        {return (this->getY() == in.getY());}

        [[maybe_unused]] bool Equl_Z(Vector3<type> in)
        {return (this->getZ() == in.getZ());}

        [[maybe_unused]] bool Equl_GreaterThan_X(Vector3<type> in)
        {return (this->getX() >= in.getX());}

        [[maybe_unused]] bool Equl_GreaterThan_Y(Vector3<type> in)
        {return (this->getY() >= in.getY());}

        [[maybe_unused]] bool Equl_GreaterThan_Z(Vector3<type> in)
        {return (this->getZ() >= in.getZ());}

        [[maybe_unused]] bool Equl_LesserThan_X(Vector3<type> in)
        {return (this->getX() <= in.getX());}

        [[maybe_unused]] bool Equl_LesserThan_Y(Vector3<type> in)
        {return (this->getY() <= in.getY());}

        [[maybe_unused]] bool Equl_LesserThan_Z(Vector3<type> in)
        {return (this->getZ() <= in.getZ());}

        [[maybe_unused]] bool GreaterThan_X(Vector3<type> in)
        {return (this->getX() > in.getX());}

        [[maybe_unused]] bool GreaterThan_Y(Vector3<type> in)
        {return (this->getY() > in.getY());}

        [[maybe_unused]] bool GreaterThan_Z(Vector3<type> in)
        {return (this->getZ() > in.getZ());}

        [[maybe_unused]] bool LesserThan_X(Vector3<type> in)
        {return (this->getX() < in.getX());}

        [[maybe_unused]] bool LesserThan_Y(Vector3<type> in)
        {return (this->getY() < in.getY());}

        [[maybe_unused]] bool LesserThan_Z(Vector3<type> in)
        {return (this->getZ() < in.getZ());}

        [[maybe_unused]] void Add_X(Vector3<type> in)
        {this->getX() + in.getX();}

        [[maybe_unused]] void Add_Y(Vector3<type> in)
        {this->getY() + in.getY();}

        [[maybe_unused]] void Add_Z(Vector3<type> in)
        {this->getZ() + in.getZ();}

        [[maybe_unused]] void Sub_X(Vector3<type> in)
        {this->getX() - in.getX();}

        [[maybe_unused]] void Sub_Y(Vector3<type> in)
        {this->getY() - in.getY();}

        [[maybe_unused]] void Sub_Z(Vector3<type> in)
        {this->getZ() - in.getZ();}

        [[maybe_unused]] void Div_X(Vector3<type> in)
        {this->getX() / in.getX();}

        [[maybe_unused]] void Div_Y(Vector3<type> in)
        {this->getY() / in.getY();}

        [[maybe_unused]] void Div_Z(Vector3<type> in)
        {this->getZ() / in.getZ();}

        [[maybe_unused]] void Mul_X(Vector3<type> in)
        {this->getX() * in.getX();}

        [[maybe_unused]] void Mul_Y(Vector3<type> in)
        {this->getY() * in.getY();}

        [[maybe_unused]] void Mul_Z(Vector3<type> in)
        {this->getZ() * in.getZ();}

        [[maybe_unused]] bool Not_Equl_X(type in)
        {return (this->getX() != in);}

        [[maybe_unused]] bool Not_Equl_Y(type in)
        {return (this->getY() != in);}

        [[maybe_unused]] bool Not_Equl_Z(type in)
        {return (this->getZ() != in);}

        [[maybe_unused]] bool Equl_X(type in)
        {return (this->getX() == in);}

        [[maybe_unused]] bool Equl_Y(type in)
        {return (this->getY() == in);}

        [[maybe_unused]] bool Equl_Z(type in)
        {return (this->getZ() == in);}

        [[maybe_unused]] bool Equl_GreaterThan_X(type in)
        {return (this->getX() >= in);}

        [[maybe_unused]] bool Equl_GreaterThan_Y(type in)
        {return (this->getY() >= in);}

        [[maybe_unused]] bool Equl_GreaterThan_Z(type in)
        {return (this->getZ() >= in);}

        [[maybe_unused]] bool Equl_LesserThan_X(type in)
        {return (this->getX() <= in);}

        [[maybe_unused]] bool Equl_LesserThan_Y(type in)
        {return (this->getY() <= in);}

        [[maybe_unused]] bool Equl_LesserThan_Z(type in)
        {return (this->getZ() <= in);}

        [[maybe_unused]] bool GreaterThan_X(type in)
        {return (this->getX() > in);}

        [[maybe_unused]] bool GreaterThan_Y(type in)
        {return (this->getY() > in);}

        [[maybe_unused]] bool GreaterThan_Z(type in)
        {return (this->getZ() > in);}

        [[maybe_unused]] bool LesserThan_X(type in)
        {return (this->getX() < in);}

        [[maybe_unused]] bool LesserThan_Y(type in)
        {return (this->getY() < in);}

        [[maybe_unused]] bool LesserThan_Z(type in)
        {return (this->getZ() < in);}

        [[maybe_unused]] void Add_X(type in)
        {this->x = this->getX() + in;}

        [[maybe_unused]] void Add_Y(type in)
        {this->y = this->getY() + in;}

        [[maybe_unused]] void Add_Z(type in)
        {this->z = this->getZ() + in;}

        [[maybe_unused]] void Sub_X(type in)
        {this->x = this->getX() - in;}

        [[maybe_unused]] void Sub_Y(type in)
        {this->y = this->getY() - in;}

        [[maybe_unused]] void Sub_Z(type in)
        {this->z = this->getZ() - in;}

        [[maybe_unused]] void Div_X(type in)
        {this->x = this->getX() / in;}

        [[maybe_unused]] void Div_Y(type in)
        {this->y = this->getY() / in;}

        [[maybe_unused]] void Div_Z(type in)
        {this->z = this->getZ() / in;}

        [[maybe_unused]] void Mul_X(type in)
        {this->x = this->getX() * in;}

        [[maybe_unused]] void Mul_Y(type in)
        {this->y = this->getY() * in;}

        [[maybe_unused]] void Mul_Z(type in)
        {this->z = this->getZ() * in;}

        Vector3<type> operator +(Vector3<type> in)
        {
            this->x = (this->getX() + in.getX());
            this->y = (this->getY() + in.getY());
            this->z = (this->getZ() + in.getZ());
            return *this;
        }

        Vector3<type> operator *(Vector3<type> in)
        {
            this->x = (this->getX() * in.getX());
            this->y = (this->getY() * in.getY());
            this->z = (this->getZ() * in.getZ());
            return *this;
        }

        Vector3<type> operator -(Vector3<type> in)
        {
            this->x = (this->getX() - in.getX());
            this->y = (this->getY() - in.getY());
            this->z = (this->getZ() - in.getZ());
            return *this;
        }

        Vector3<type> operator /(Vector3<type> in)
        {
            this->x = (this->getX() / in.getX());
            this->y = (this->getY() / in.getY());
            this->z = (this->getZ() / in.getZ());
            return *this;
        }

        bool operator <(Vector3<type> in)
        {return ((this->getX() < in.getX()) & (this->getY() < in.getY()) & (this->getZ() < in.getZ()));}

        bool operator >(Vector3<type> in)
        {return ((this->getX() > in.getX()) & (this->getY() > in.getY()) & (this->getZ() > in.getZ()));}

        Vector3<type> operator +=(Vector3<type> in)
        {
            this->x = (this->getX() += in.getX());
            this->y = (this->getY() += in.getY());
            this->z = (this->getZ() += in.getZ());
            return *this;
        }

        Vector3<type> operator *=(Vector3<type> in)
        {
            this->x = (this->getX() *= in.getX());
            this->y = (this->getY() *= in.getY());
            this->z = (this->getZ() *= in.getZ());
            return *this;
        }

        Vector3<type> operator -=(Vector3<type> in)
        {
            this->x = (this->getX() -= in.getX());
            this->y = (this->getY() -= in.getY());
            this->z = (this->getZ() -= in.getZ());
            return *this;
        }

        Vector3<type> operator /=(Vector3<type> in)
        {
            this->x = (this->getX() /= in.getX());
            this->y = (this->getY() /= in.getY());
            this->z = (this->getZ() /= in.getZ());
            return *this;
        }

        bool operator <=(Vector3<type> in)
        {return ((this->getX() <= in.getX()) & (this->getY() <= in.getY()) & (this->getZ() <= in.getZ()));}

        bool operator >=(Vector3<type> in)
        {return ((this->getX() >= in.getX()) & (this->getY() >= in.getY()) & (this->getZ() >= in.getZ()));}

        bool operator ==(Vector3<type> in)
        {return ((this->getX() == in.getX()) & (this->getY() == in.getY()) & (this->getZ() == in.getZ()));}

        bool operator !=(Vector3<type> in)
        {return ((this->getX() != in.getX()) & (this->getY() != in.getY()) & (this->getZ() != in.getZ()));}
    };
}
#endif //LUNALUXMATHLIB_VECTOR_HPP
