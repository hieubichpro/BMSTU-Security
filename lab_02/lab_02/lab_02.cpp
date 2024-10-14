// lab_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <bitset>
#include <bit>
#include <cstdint>
#include <fstream>

using namespace std;

class DES
{
public:
    bitset<64> process_block(bitset<64> value, bitset<64> key, bool decrypte = false)
    {
        // generate keys
        auto keys = generate_keys(key, decrypte);

        // initial permutation
        auto round_val = IP_f(value);

        // 16 rounds
        for (auto rkey : keys) {
            round_val = wround(round_val, rkey);
        }

        // final permutation
        auto final_val = FP_f(round_val);

        return final_val;
    }
protected:
    // initial permutation table
    int IP[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9,	1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };
    // final permutation table
    int FP[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9 , 49, 17, 57, 25
    };

    // permuted choice 1
    int PC1[56] = {
            57, 49, 41, 33, 25, 17, 9 ,
            1 , 58, 50, 42, 34, 26, 18,
            10, 2 , 59, 51, 43, 35, 27,
            19, 11, 3 , 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15,
            7 , 62, 54, 46, 38, 30, 22,
            14, 6 , 61, 53, 45, 37, 29,
            21, 13, 5 , 28, 20, 12, 4
    };

    // permuted choice 2
    int PC2[48] = {
            14, 17, 11, 24, 1 , 5 ,
            3 , 28, 15, 6 , 21, 10,
            23, 19, 12, 4 , 26, 8 ,
            16, 7 , 27, 20, 13, 2 ,
            41, 52, 31, 37, 47, 55,
            30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53,
            46, 42, 50, 36, 29, 32
    };

    // expansion function table
    int E[48] = {
            32, 1 , 2 , 3 , 4 , 5 ,
            4 , 5 , 6 , 7 , 8 , 9 ,
            8 , 9 , 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32, 1
    };

    // S-box tables
    int S[8][4][16] = { {
                                { 14, 4 , 13, 1, 2 , 15, 11, 8 , 3 , 10, 6 , 12, 5 , 9 , 0, 7 },
                                { 0 , 15, 7 , 4, 14, 2 , 13, 1 , 10, 6 , 12, 11, 9 , 5 , 3, 8 },
                                { 4 , 1 , 14, 8, 13, 6 , 2 , 11, 15, 12, 9 , 7 , 3 , 10, 5, 0 },
                                { 15, 12, 8 , 2, 4 , 9 , 1 , 7 , 5 , 11, 3 , 14, 10, 0 , 6, 13}
                        }, {
                                {15, 1 , 8 , 14, 6 , 11, 3 , 4 , 9 , 7, 2 , 13, 12, 0, 5 , 10},
                                {3 , 13, 4 , 7 , 15, 2 , 8 , 14, 12, 0, 1 , 10, 6 , 9, 11, 5 },
                                {0 , 14, 7 , 11, 10, 4 , 13, 1 , 5 , 8, 12, 6 , 9 , 3, 2 , 15},
                                {13, 8 , 10, 1 , 3 , 15, 4 , 2 , 11, 6, 7 , 12, 0 , 5, 14, 9 }
                        }, {
                                {10, 0 , 9 , 14, 6, 3 , 15, 5 , 1 , 13, 12, 7 , 11, 4 , 2 , 8 },
                                {13, 7 , 0 , 9 , 3, 4 , 6 , 10, 2 , 8 , 5 , 14, 12, 11, 15, 1 },
                                {13, 6 , 4 , 9 , 8, 15, 3 , 0 , 11, 1 , 2 , 12, 5 , 10, 14, 7 },
                                {1 , 10, 13, 0 , 6, 9 , 8 , 7 , 4 , 15, 14, 3 , 11, 5 , 2 , 12}
                        }, {
                                {7 , 13, 14, 3, 0 , 6 , 9 , 10, 1 , 2, 8, 5 , 11, 12, 4 , 15},
                                {13, 8 , 11, 5, 6 , 15, 0 , 3 , 4 , 7, 2, 12, 1 , 10, 14, 9 },
                                {10, 6 , 9 , 0, 12, 11, 7 , 13, 15, 1, 3, 14, 5 , 2 , 8 , 4 },
                                {3 , 15, 0 , 6, 10, 1 , 13, 8 , 9 , 4, 5, 11, 12, 7 , 2 , 14}
                        }, {
                                {2 , 12, 4 , 1 , 7 , 10, 11, 6 , 8 , 5 , 3 , 15, 13, 0, 14, 9 },
                                {14, 11, 2 , 12, 4 , 7 , 13, 1 , 5 , 0 , 15, 10, 3 , 9, 8 , 6 },
                                {4 , 2 , 1 , 11, 10, 13, 7 , 8 , 15, 9 , 12, 5 , 6 , 3, 0 , 14},
                                {11, 8 , 12, 7 , 1 , 14, 2 , 13, 6 , 15, 0 , 9 , 10, 4, 5 , 3 }
                        }, {
                                {12, 1 , 10, 15, 9, 2 , 6 , 8 , 0 , 13, 3 , 4 , 14, 7 , 5 , 11},
                                {10, 15, 4 , 2 , 7, 12, 9 , 5 , 6 , 1 , 13, 14, 0 , 11, 3 , 8 },
                                {9 , 14, 15, 5 , 2, 8 , 12, 3 , 7 , 0 , 4 , 10, 1 , 13, 11, 6 },
                                {4 , 3 , 2 , 12, 9, 5 , 15, 10, 11, 14, 1 , 7 , 6 , 0 , 8 , 13}
                        }, {
                                {4 , 11, 2 , 14, 15, 0, 8 , 13, 3 , 12, 9, 7 , 5 , 10, 6, 1 },
                                {13, 0 , 11, 7 , 4 , 9, 1 , 10, 14, 3 , 5, 12, 2 , 15, 8, 6 },
                                {1 , 4 , 11, 13, 12, 3, 7 , 14, 10, 15, 6, 8 , 0 , 5 , 9, 2 },
                                {6 , 11, 13, 8 , 1 , 4, 10, 7 , 9 , 5 , 0, 15, 14, 2 , 3, 12}
                        }, {
                                {13, 2 , 8 , 4, 6 , 15, 11, 1 , 10, 9 , 3 , 14, 5 , 0 , 12, 7 },
                                {1 , 15, 13, 8, 10, 3 , 7 , 4 , 12, 5 , 6 , 11, 0 , 14, 9 , 2 },
                                {7 , 11, 4 , 1, 9 , 12, 14, 2 , 0 , 6 , 10, 13, 15, 3 , 5 , 8 },
                                {2 , 1 , 14, 7, 4 , 10, 8 , 13, 15, 12, 9 , 0 , 3 , 5 , 6 , 11}
                        }
    };

    // permutation
    int P[32] = {
            16, 7 , 20, 21, 29, 12, 28, 17,
            1 , 15, 23, 26, 5 , 18, 31, 10,
            2 , 8 , 24, 14, 32, 27, 3 , 9 ,
            19, 13, 30, 6 , 22, 11, 4 , 25
    };

    // how to shift
    int key_shift_cnt[16] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
    };

    bitset<56> PC1_f(bitset<64> key)
    {
        bitset<56> res = { 0 };
        int i = 0;
        for (auto index : PC1)
        {
            res[i] = key[index - 1];
            i++;
        }
        return res;
    }
    bitset<48> PC2_f(bitset<56> key)
    {
        bitset<48> res = { 0 };
        int i = 0;
        for (auto index : PC2)
        {
            res[i] = key[index - 1];
            i++;
        }
        return res;
    }
    vector<bitset<48>> generate_keys(bitset<64> key, bool decrypte = false)
    {
        auto after_PC1 = PC1_f(key);
        vector<bitset<48>> res = {};

        for (auto shift : key_shift_cnt)
        {
            auto tmp = after_PC1;

            bitset<28> left = {};
            bitset<28> right = {};

            for (int i = 0; i < 56; i++) {
                if (i < 28) {
                    left[i] = tmp[i];
                }
                else {
                    right[i - 28] = tmp[i];
                }
            }
            auto initial_left = left;
            auto initial_right = right;

            // cycle shift
            left <<= shift;
            int j = 0;
            for (int i = 28 - shift; i < 28; i++) {
                left[i] = initial_left[j];
                j++;
            }

            right <<= shift;
            j = 0;
            for (int i = 28 - shift; i < 28; i++) {
                right[i] = initial_right[j];
                j++;
            }

            for (int i = 0; i < 28; i++) {
                after_PC1[i] = left[i];
                after_PC1[i + 27] = right[i];
            }
            res.push_back(PC2_f(after_PC1));
        }
        if (decrypte)
            reverse(res.begin(), res.end());
        return res;

    }
    pair<bitset<32>, bitset<32>> IP_f(bitset<64> value)
    {
        bitset<64> after_ip = {};
        int i = 0;
        for (auto index : IP)
        {
            after_ip[i] = value[index - 1];
            i++;
        }

        bitset<32> left = {};
        bitset<32> right = {};

        for (int i = 0; i < 32; i++) {
            left[i] = after_ip[i];
            right[i] = after_ip[i + 32];
        }

        return pair<bitset<32>, bitset<32>>(left, right);
    }

    pair<bitset<32>, bitset<32>> wround(pair<bitset<32>, bitset<32>> RL, bitset<48> key)
    {
        return pair<bitset<32>, bitset<32>>(RL.second, RL.first ^ inner_f(RL.second, key));
    }

    bitset<48> E_f(bitset<32> value)
    {
        bitset<48> res = { 0 };
        int i = 0;
        for (auto index : E)
        {
            res[i] = value[index - 1];
            i++;
        }
        return res;
    }
    bitset<4> S_f(bitset<6> value, int i)
    {
        bitset<2> row_b = {};
        row_b[0] = value[0];
        row_b[1] = value[5];

        bitset<4> col_b = {};
        col_b[0] = value[1];
        col_b[1] = value[2];
        col_b[2] = value[3];
        col_b[3] = value[4];

        int row = row_b.to_ulong();
        int col = col_b.to_ulong();

        bitset<4> result = {};

        result = S[i][row][col];
        return result;
    }
    bitset<32> P_f(bitset<32> value)
    {
        bitset<32> res = { 0 };
        int i = 0;
        for (auto index : P)
        {
            res[i] = value[index - 1];
            i++;
        }
        return res;
    }
    bitset<32> inner_f(bitset<32> value, bitset<48> key)
    {
        auto expanded_val = E_f(value);
        auto xor_res = key ^ expanded_val;

        bitset<6> tmp = {};
        bitset<32> result = {};

        int k = 0;
        for (int i = 0; i < 8; i += 1) {
            // block of 6 bits
            for (int j = 0; j < 6; j++) {
                tmp[j] = xor_res[i * 6 + j];
            }

            auto s_res = S_f(tmp, i);

            for (int p = 0; p < 4; p++) {
                result[p + k] = s_res[p];
            }
            k += 4;
        }

        auto final_res = P_f(result);

        return final_res;
    }
    bitset<64> FP_f(pair<bitset<32>, bitset<32>> RL)
    {
        bitset<64> tmp_res = {};

        for (int i = 0; i < 32; i++) {
            tmp_res[i] = RL.second[i];
            tmp_res[i + 32] = RL.first[i];
        }

        int i = 0;
        bitset<64> result = {};
        for (auto index : FP) {
            result[i] = tmp_res[index - 1];
            i++;
        }

        return result;
    }
};

class CBC {
public:
    CBC() {};

    string cypher(string input, string key, string vi)
    {
        std::vector<char> idata(input.begin(), input.end());
        std::vector<char> keydata(key.begin(), key.end());
        vector<char> vidata(vi.begin(), vi.end());
        auto result = cypher(idata, keydata, vidata);

        std::string str(result.begin(), result.end());

        return str;
    }
    vector<char> cypher(vector<char> input, vector<char> key, vector<char> vi)
    {
        vector<char> buffer = {};
        vector<char> result = {};

        int last_cnt = 0;

        auto vi_initial = vchar_to_bitset64(vi);
        auto key_b = vchar_to_bitset64(key);


        for (auto sym : input) {
            if (buffer.size() < 8) {
                buffer.push_back(sym);
            }

            if (buffer.size() == 8) {
                // buffer size is 8 -> can be cyphered
                auto buf_b = vchar_to_bitset64(buffer) ^ vi_initial;

                auto tmp_b_1 = _des.process_block(buf_b, key_b);

                vi_initial = tmp_b_1;

                auto tmp_res = bitset64_to_vchar(tmp_b_1);

                result.insert(result.end(), tmp_res.begin(), tmp_res.end());

                buffer.clear();
            }
        }

        if (buffer.size() > 0 && buffer.size() < 8) {
            while (buffer.size() < 8) {
                buffer.push_back((char)0);
                last_cnt += 1;
            }
            auto buf_b = vchar_to_bitset64(buffer) ^ vi_initial;

            auto tmp_b_1 = _des.process_block(buf_b, key_b);

            vi_initial = tmp_b_1;

            auto tmp_res = bitset64_to_vchar(tmp_b_1);

            result.insert(result.end(), tmp_res.begin(), tmp_res.end());
        }

        result.push_back((char)last_cnt);


        return result;
    }


    string decypher(string input, string key, string vi)
    {
        std::vector<char> idata(input.begin(), input.end());
        std::vector<char> keydata(key.begin(), key.end());
        vector<char> vidata(vi.begin(), vi.end());
        auto result = decypher(idata, keydata, vidata);

        std::string str(result.begin(), result.end());

        return str;
    }
    vector<char> decypher(vector<char> input, vector<char> key, vector<char> vi)
    {
        vector<char> buffer = {};
        vector<char> result = {};

        int last_cnt = input.back();
        input.pop_back();

        auto vi_initial = vchar_to_bitset64(vi);
        auto key_b = vchar_to_bitset64(key);


        for (auto sym : input) {
            if (buffer.size() < 8) {
                buffer.push_back(sym);
            }

            if (buffer.size() == 8) {
                // buffer size is 8 -> can be cyphered
                auto buf_b = vchar_to_bitset64(buffer);

                auto tmp_b_1 = _des.process_block(buf_b, key_b, true) ^ vi_initial;

                vi_initial = buf_b;

                auto tmp_res = bitset64_to_vchar(tmp_b_1);

                result.insert(result.end(), tmp_res.begin(), tmp_res.end());

                buffer.clear();
            }
        }

        if (buffer.size() > 0 && buffer.size() < 8) {
            while (buffer.size() < 8) {
                buffer.push_back((char)0);
                last_cnt += 1;
            }
            auto buf_b = vchar_to_bitset64(buffer);

            auto tmp_b_1 = _des.process_block(buf_b, key_b, true) ^ vi_initial;

            vi_initial = buf_b;

            auto tmp_res = bitset64_to_vchar(tmp_b_1);

            result.insert(result.end(), tmp_res.begin(), tmp_res.end());
        }


        
        for (int i = 0; i < last_cnt; i++) 
            result.pop_back();


        return result;
    }



    static bitset<64> vchar_to_bitset64(vector<char> input)
    {
        bitset<64> val = { 0 };

        reverse(input.begin(), input.end());

        for (auto sym : input) {
            val <<= 8;
            bitset<8> tmp = sym;

            for (int i = 0; i < 8; i++) {
                val[i] = tmp[7 - i];
            }
        }

        return val;
    }

    static vector<char> bitset64_to_vchar(bitset<64> input)
    {
        vector<char> val = { };

        for (int i = 0; i < 64; i += 8) {
            char tmp = 0;
            for (int j = i; j < i + 8; j++) {
                tmp <<= 1;
                tmp |= input[j];
            }

            val.push_back(tmp);
        }

        return val;
    }
private:
    DES _des;
};

vector<char> read_bytes_from_file(string fname)
{
    ifstream in(fname, ios::binary);

    vector<char> buffer;
    char tmp;
    if (!in.is_open())
    {
        cerr << "Не удалось открыть файл!" << endl;
        return {};
    }
    while (in.get(tmp))
        buffer.push_back(tmp);
    in.close();

    return buffer;
}

void write_bytes_into_file(string fname, vector<char> data) {
    ofstream out;
    out.open(fname, ios::binary);
    //if (!out.is_open())
    //{
    //    cerr << "Не удалось открыть файл!" << endl;
    //    return;
    //}
    for (auto sym : data)
        out.put(sym);
    out.close();
}

int main()
{
    CBC cbc;
    string message = "HelloEnigma123456FromMosckvaRussia";
    //string key = "2a4c1123";
    //string vi = "12a3v4sh";
    vector<char> key = { '2', 'a', '4', 'c', '1', '1' , '2', '3'};
    vector<char> vi = { '1', 'a', 't', 'c', 's', 'h' , 'a', 'b'};
    //auto encryte = cbc.cypher(message, key, vi);
    //cout << "p1 " << encryte << endl;
    //cout << "p2 " << cbc.decypher(encryte, key, vi);



    //vector<char> test = read_bytes_from_file("a.pdf");
    //cout << test.size() << endl;
    //auto encrypt_text = cbc.cypher(test, key, vi);
    //write_bytes_into_file("a.enc", encrypt_text);

    //auto decrypt_text = cbc.decypher(encrypt_text, key, vi);
    //write_bytes_into_file("res.pdf", decrypt_text);



    vector<char> test = read_bytes_from_file("test.zip");
    cout << test.size() << endl;
    auto encrypt_text = cbc.cypher(test, key, vi);
    write_bytes_into_file("test.enc", encrypt_text);

    auto decrypt_text = cbc.decypher(encrypt_text, key, vi);
    write_bytes_into_file("test2.zip", decrypt_text);


    cout << "all runned ok" << endl;

    return 0;


}
