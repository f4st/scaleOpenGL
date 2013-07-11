//
//  Matrix.h
//  StartMeeting
//
//  Created by Mykola Farion on 4/26/13.
//  Copyright (c) 2013 WYDEVoice. All rights reserved.
//

#ifndef __StartMeeting__Matrix__
#define __StartMeeting__Matrix__

#include <iostream>

#endif /* defined(__StartMeeting__Matrix__) */


class Matrix4f
{
public:
    
    float** p;
    float* p1D;
    // constructor
    Matrix4f(): p (NULL), p1D(NULL), rows(4), cols(4)
    {
        // create a Matrix 4x4 with ones on diagonal;
        p = new float*[rows];
        for (int r = 0; r < rows; r++)
        {
            p[r] = new float[cols];
            for (int c = 0; c < cols; c++)
            {
                if(r == c) {
                    p[r][c] = 1;
                } else {
                    p[r][c] = 0;
                }
            }
        }
    }
    
    // destructor
    /*~Matrix4f()
     {
     for (int r = 0; r < rows; r++)
     {
     delete p[r];
     }
     delete p1D;
     delete p;
     p = NULL;
     }*/
    
    Matrix4f transponse()
    {
        Matrix4f tmp;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                tmp.p[j][i] =  p[i][j];
            }
        }
        return tmp;
    }
    
    float* pointer1D(void)
    {
        int position;
        p1D = new float[rows*cols];
        for (int i = 0; i < rows; i++)
        {
            position = i*cols;
            for (int j = 0; j < cols; j++)
            {
                p1D[position] = p[i][j];
                position++;
            }
        }
        return p1D;
    }
    // operator multiplication
    friend Matrix4f operator* (const Matrix4f& a, const Matrix4f& b)
    {
        Matrix4f res;
        for (int r = 0; r < a.rows; r++)
        {
            for (int c_res = 0; c_res < b.cols; c_res++)
            {
                res.p[r][c_res] = 0;
                for (int c = 0; c < a.cols; c++)
                {
                    res.p[r][c_res] += a.p[r][c] * b.p[c][c_res];
                }
            }
        }
        return res;
    }
    
    Matrix4f addParams (const Matrix4f& scaleM, const Matrix4f& transM)
    {
        
        Matrix4f res;
        for (int i = 0; i < res.rows; i++)
        {
            for (int j = 0; j < res.cols; j++)
            {
                if(i==(res.cols-1) && (j!=4))
                {
                    res.p[i][j] = transM.p[i][j];
                }
                else
                {
                    res.p[i][j] = scaleM.p[i][j];
                }
            }
        }
        return res;
    }
    
    Matrix4f genScaleMatrix(float x, float y/*, float z*/)
    {
        Matrix4f res;
        for(int i = 0; i < res.rows;i++)
        {
            for (int j = 0; j < res.cols; j++)
            {
                res.p[i][j] = 0;
            }
        }
        res.p[0][0] = x;
        res.p[1][1] = y;
        //res.p[2][2] = z;
        res.p[3][3] = 1;
        return res;
    }
    
    Matrix4f genTranslationMatrix(float x, float y/*, float z*/)
    {
        Matrix4f res;
        for(int i = 0; i < res.rows;i++)
        {
            for (int j = 0; j < res.cols; j++)
            {
                if(i == j)
                {
                    res.p[i][j] = 1;
                }
                else
                {
                    res.p[i][j] = 0;
                }
            }
        }
        res.p[3][0] = x;
        res.p[3][1] = y;
        //res.p[3][2] = z;
        return res;
    }
    
    
private:
    int rows;
    int cols;
    
};

