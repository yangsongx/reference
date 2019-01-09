#include <iostream>

#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct final_result{
    string fr_name;
    string fr_domain;
};

class Solution {
    size_t m_idx;
    vector<struct final_result> m_result;

    public:
        int append_uniquely(struct final_result &input) {
            int unique = 1;
            for (vector<struct final_result>::iterator it = m_result.begin();
                    it != m_result.end();
                    it++)
            {
                printf("comparing...\n");
                if(input.fr_name == it->fr_name && input.fr_domain == it->fr_domain){
                    unique = 0;
                    break;
                }
            }
            if(unique == 1){
                printf("==need insert it\n");
                m_result.push_back(input);
            }

            return 0;
        }

        int numUniqueEmails(vector<string>& emails) {
            int i;
            char buffer[200];
            for (vector<string>::iterator it = emails.begin();
                    it != emails.end();
                    it++) {
                struct final_result item;

                m_idx = it->find("@");
                string name = it->substr(0, m_idx);
                string name2 = name;
                item.fr_domain = it->substr(m_idx + 1);
                printf("email:%s, index=%d, name:%s, domain:%s\n",
                        it->c_str(), m_idx, name.c_str(), item.fr_domain.c_str());
                // first, remove the +
                m_idx = name.find("+");
                if(m_idx != string::npos) {
                    name2 = name.substr(0, m_idx);
                }
                

                i = 0;
                strcpy(buffer, name2.c_str());

                while(buffer[i] != '\0') {
                    if(buffer[i] == '.') {
                        if(i == 0) {
                            printf(". is leading char\n");
                            break;
                        } else {
                            strcpy((buffer + i), (const char *)(buffer + i + 1));
                        }
                    }

                    i++;
                }

                printf("after handling, %s --> %s\n", name.c_str(), buffer);
                item.fr_name = buffer;
                append_uniquely(item);

            }

            return m_result.size();
        }
};

int main(int argc, char **argv)
{
    Solution s;
    vector<string> email_list;
    email_list.push_back("test.email+alex@leetcode.com");
    email_list.push_back("test.e.mail+bob.cathy@leetcode.com");
    email_list.push_back("testemail+david@lee.tcode.com");
    printf("the return value of test: %d\n", s.numUniqueEmails(email_list));
    return 0;
}
