#ifndef STATUS_H
#define STATUS_H

enum Status { Off, On };

constexpr Status operator!(Status status) { return status == On ? Off : On; }

constexpr Status toStatus(char const *const string) { return string[0] == 'o' && string[1] == 'n' ? On : Off; }

#endif // STATUS_H
