#include "sha1.h"

#include<iostream>
#include<vector>
#include<string>

using std::string;
using std::vector;

namespace SHA1 {
    vector<int> X;//8*64=512，每个下标存放8位
    int W[80];//32位为一组
    int A, B, C, D, E;
    int A1, B1, C1, D1, E1;//缓冲区寄存器,产生最后结果
    int Turn;//加密分组数量

    void printX() {//输出填充后的文本
        for (int i = 0; i < X.size(); i++) {
            printf("%02x", X[i]);
            if ((i + 1) % 4 == 0)
                printf(" ");
            if ((i + 1) % 16 == 0)
                printf("\n");
        }
    }

    int S(unsigned int x, int n) {//循环左移
        return x >> (32 - n) | (x << n);
    }

    void append(string m) {//文本的填充处理
        Turn = (m.size() + 8) / 64 + 1;
        X.resize(Turn * 64);
        int i = 0;
        for (; i < m.size(); i++) {
            X[i] = m[i];
        }
        X[i++] = 0x80;
        while (i < X.size() - 8) {
            X[i] = 0;
            i++;
        }
        long long int a = m.size() * 8;
        for (i = X.size() - 1; i >= X.size() - 8; i--) {
            X[i] = a % 256;
            a /= 256;
        }
    }

    void setW(vector<int> m, int n) {//W数组的生成
        n *= 64;
        for (int i = 0; i < 16; i++) {
            W[i] = (m[n + 4 * i] << 24) + (m[n + 4 * i + 1] << 16)
                + (m[n + 4 * i + 2] << 8) + m[n + 4 * i + 3];
        }
        for (int i = 16; i < 80; i++) {
            W[i] = S(W[i - 16] ^ W[i - 14] ^ W[i - 8] ^ W[i - 3], 1);
        }
    }

    int ft(int t) {//ft(B,C,D)函数
        if (t < 20)
            return (B & C) | ((~B) & D);
        else if (t < 40)
            return B ^ C ^ D;
        else if (t < 60)
            return (B & C) | (B & D) | (C & D);
        else
            return B ^ C ^ D;
    }

    int Kt(int t) {//常量K
        if (t < 20)
            return 0x5a827999;
        else if (t < 40)
            return 0x6ed9eba1;
        else if (t < 60)
            return 0x8f1bbcdc;
        else
            return 0xca62c1d6;
    }

    string sha1(string text) {
        A1 = A = 0x67452301;
        B1 = B = 0xefcdab89;
        C1 = C = 0x98badcfe;
        D1 = D = 0x10325476;
        E1 = E = 0xc3d2e1f0;
        append(text);
        // printX();
        for (int i = 0; i < Turn; i++) {
            setW(X, i);
            for (int t = 0; t < 80; t++) {
                int temp = E + ft(t) + S(A, 5) + W[t] + Kt(t);
                E = D;
                D = C;
                C = S(B, 30);
                B = A;
                A = temp;
            }
            A1 = A = A + A1;
            B1 = B = B + B1;
            C1 = C = C + C1;
            D1 = D = D + D1;
            E1 = E = E + E1;
        }

        char str[41];
        sprintf(str, "%08x%08x%08x%08x%08x", A1, B1, C1, D1, E1);
        string ret(str);
        return str;
    }

    // string sha1(string path, vector<string> content) {
    //     string text = path;
    //     for (auto line : content) {
    //         text += line;
    //     }
    //     return sha1(text);
    // }

    string sha1(string s1, string s2) {
        return sha1(s1 + s2);
    }

    string sha1(string s1, string s2, string s3, string s4) {
        return sha1(s1 + s2 + s3 + s4);
    }
};
