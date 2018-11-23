# python 3
from unireedsolomon import rs
from math import ceil, log
import numpy as np

IMAGE = "timg.jpg"
PACKET_SIZE = 980

def rs_encode_image(image_file, code_word_num):
    with open(image_file, 'rb') as f:
        image_contain = f.read()
    k = ceil(len(image_contain) / float(PACKET_SIZE))
    n = 2 ** ceil(log(k) / log(2))
    fout = open(IMAGE+'.rs', 'wb')
    mat = []
    for i in range(k):
        if(len(image_contain) > (i+1) * PACKET_SIZE):
            mat.append([str(ii) for ii in list(image_contain[i * PACKET_SIZE : (i+1) * PACKET_SIZE])])
        else:
            empty = ['0' for ii in range(PACKET_SIZE)]
            empty[:(len(image_contain) - i * PACKET_SIZE)] = list(image_contain[i * PACKET_SIZE : len(image_contain)])
            mat.append(empty)
    for i in range(n-k):
        mat.append(['0' for ii in range(PACKET_SIZE)])
    mat_arr_orig = np.array(mat)
    mat_arr_code = mat_arr_orig

    coder = rs.RSCoder(n, k)
    for i in range(PACKET_SIZE):
        code_word = list(coder.encode(bytes([int(ii) for ii in mat_arr_orig[:k, i]])))
        mat_arr_code[:, i] = code_word
     
    for line in mat_arr_code:
        fout.write(bytes(''.join(list(line)), encoding='utf-8'))
    fout.close()           
    
    return mat_arr_orig, image_contain, mat, mat_arr_code

if __name__ == "__main__":
    mat_arr_orig, image_contain, mat, mat_arr_code = rs_encode_image(IMAGE, 1)

