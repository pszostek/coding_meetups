//
// http://www.careercup.com/question?id=5694329337151488
//
#include <string>
#include <iostream>

void swap_pairwise(std::string& str, size_t first, size_t last) {
    if(last == std::string::npos || last >= str.length())
        last = str.length()-1;
    while(first < last) {
       char temp = str[first];
       str[first] = str[last];
       str[last] = temp;
       ++first; --last; 
    }
}

void reverse_string(std::string& str) {
    size_t prev_space = -1;
    size_t cur_space = 0;
    do {
        cur_space = str.find(' ', prev_space+1);
        if(cur_space == str.length()-1)
            break;
        swap_pairwise(str, prev_space+1, cur_space-1);
        prev_space = cur_space;
    } while(cur_space != std::string::npos);
}


int main() {
    std::string example = "this is an example";
    reverse_string(example);
    std::cout << example << std::endl;
}