// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include<string>

template<typename T,size_t Height ,size_t Weidth,typename Alloc =std::allocator<T>>
class Matrix {
private:
    size_t hg;
    size_t wh;
    T** mat;
    Alloc alloc;
    
    using AllocTraits = std::allocator_traits<Alloc>;

    
public:
    //begin construc and destruct 
    Matrix(Alloc alloc = Alloc()) :hg(Height), wh(Weidth), alloc(alloc) {
        mat = new T * [hg];
        for (int i = 0; i < hg; ++i) {
            *(mat + i) = AllocTraits::allocate(alloc,wh);
            //*(mat + i) = alloc.allocate(wh);
        }
    }
    Matrix(T val) :Matrix() {
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                AllocTraits::construct(alloc, *(mat + i) + j, val);
                //alloc.construct(*(mat + i) + j, val);
            }
        }
   }
    Matrix(const Matrix& a):Matrix() {
        
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                AllocTraits::construct(alloc, *(mat + i) + j, *(*(a.mat + i) + j));
                //alloc.construct(*(mat + i) + j, *(* (a.mat+ i) + j));
            }
        }
    }
    Matrix(Matrix&& a) :Matrix() {

        mat = a.mat;
        a.mat = nullptr;
    }
    ~Matrix() {
        clear();
        for (int i = 0; i < hg; ++i) {
            AllocTraits::deallocate(alloc, *(mat + i), wh);
            //alloc.deallocate(*(mat + i), wh);

        }
        delete[] mat;

    }
    //end construct and sestruct

    //begin opeartor
   


    const Matrix operator*(const T& val) {
        Matrix copy = *this;
        copy *= val;
        return copy;
    }
    Matrix& operator=(const Matrix& a ) {
        Matrix copy = a;
        
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                std::swap(a[i][j],mat[i][j]);
            }
        }
        return *this;
    } 
    Matrix& operator=(Matrix&& a) {
        mat = a.mat;
        a.mat = nullptr;
        return *this;
    }
    
    T*& operator[](size_t i) {
        return *(mat + i);
    }
    T* operator[](size_t i) const{
        return *(mat + i) ;
    }
    Matrix& operator*=(const T& val) {
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                mat[i][j] *= val;
            }
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& a) {
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                mat[i][j] -= a[i][j];
            }
        }
        return *this;
    }
    Matrix& operator+=(const Matrix& a) {
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                mat[i][j] +=a[i][j];
            }
        }
        return *this ;
    }
    const Matrix operator-(const Matrix& a) {
        Matrix newarr = *this;
        newarr -= a;
        return newarr;
    }
    const Matrix operator+(const Matrix& a) {
        Matrix newarr=*this;
        newarr += a;
        return newarr;
    }

    //end operator
   
    //begin methods
    double det() {
        //algroritm determiant ?
        const double EPS = 1E-9;
        double det = 1;
        Matrix copy = *this;
        for (int i = 0; i <hg ; ++i) {
            int k = i;
            for (int j = i + 1; j < wh; ++j)
                if (abs(copy[j][i]) > abs(copy[k][i]))
                    k = j;
            if (abs(copy[k][i]) < EPS) {
                det = 0;
                break;
            }
            std::swap(copy[i], copy[k]);
            if (i != k)
                det = -det;
            det *= copy[i][i];
            for (int j = i + 1; j < wh; ++j)
                copy[i][j] /= copy[i][i];
            for (int j = 0; j < wh; ++j)
                if (j != i && abs(copy[j][i]) > EPS)
                    for (int k = i + 1; k < hg; ++k)
                        copy[j][k] -= copy[i][k] * copy[j][i];
        }
        return det;

    }
    void construct(T* ptr, T val) {
        AllocTraits::construct(alloc, ptr, val);
        //alloc.construct(ptr, val);
    }
    size_t weight() const {
        return wh;
    }
    size_t hegth() const  {
        return hg;
    }
    void clear() {
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                AllocTraits::destroy(alloc, *(mat + i) + j);
                //alloc.destroy(*(mat + i) + j);
            }
            
        }
     
      
    }
    void print() const {
        for (int i = 0; i < hg; ++i) {
            for (int j = 0; j < wh; ++j) {
                std::cout << *(*(mat + i) + j) << " ";
            }
            std::cout << "\n";
        }
    }
    friend std::istream& operator>>(std::istream& in, Matrix& a) {
        a.clear();
        T val;
        for (int i = 0; i < a.hegth(); ++i) {
            for (int j = 0; j < a.weight(); ++j) {
                in >> val;
                a[i][j] = val;
            }
        }
        
        return in;
    }
    //end nethods
};
template<typename T,size_t M,size_t N>
std::ostream& operator<<(std::ostream& out, const Matrix<T, M, N>& a)   {
    for (int i = 0; i <a.hegth(); ++i) {
        for (int j = 0; j < a.weight(); ++j) {
            out <<a[i][j] << " ";
        }
        out << "\n";
    }
    return out;
}
template<typename T, size_t M, size_t N>
const Matrix<T, M, N> operator-(const T& val, const Matrix<T, M, N>& a) {

    Matrix<T, M, N> newarr = -1*a;
    newarr += val;

    return newarr;

}
template<typename T, size_t M, size_t N>
const Matrix<T, M, N> operator+(const T& val, const Matrix<T, M, N>& a) {

    Matrix<T, M, N> newarr = a;
    newarr += val;

    return newarr;

}
template<typename T, size_t M, size_t N>
const Matrix<T, M, N> operator*(const T& val, const Matrix<T, M,N>& a) {
    
    Matrix<T, M, N> newarr = a;
    newarr *= val;
    
    return newarr;

}
template<typename T,size_t M,size_t N,size_t K>
const Matrix<T, M, K> operator*(const Matrix<T, M, N>& a, const Matrix<T, N, K>& b) {
    Matrix<T, M, K> newarr;
   
    for (int k = 0; k < b.weight(); ++k) {
        T sum = 0;
        for (int i = 0; i < a.hegth(); ++i) {
            for (int j = 0; j < a.weight(); ++j) {
                sum += a[i][j] * b[j][k];
            }
            newarr.construct(newarr[i]+k, sum);
            
        }
        
    }
    return newarr; 

}

int main()
{
    Matrix <int, 2, 2> d(1);
    std::cin >> d;
    std::cout << d<<"\n";
    std::cout << d.det();
    Matrix<int,2,3> a(1);
    Matrix<int, 2, 3> c(2);
    Matrix<int, 3, 5> b(2);
    Matrix<int, 2, 5> m(0);
    

    

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
