#include <stdlib.h>
#include <avr/pgmspace.h>
#include "font.h"
const unsigned char font6x8_char_table[] PROGMEM = {
// ' ' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// '!' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x38,  // ..###.
0x38,  // ..###.
0x10,  // ...#..
0x10,  // ...#..
0x00,  // ......
0x10,  // ...#..
0x00,  // ......
// '"' Width:6 Height:8 Bytes:1
0x6c,  // .##.##
0x6c,  // .##.##
0x48,  // .#..#.
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// '#' Width:6 Height:8 Bytes:1
0x00,  // ......
0x28,  // ..#.#.
0x7c,  // .#####
0x28,  // ..#.#.
0x28,  // ..#.#.
0x7c,  // .#####
0x28,  // ..#.#.
0x00,  // ......
// '$' Width:6 Height:8 Bytes:1
0x20,  // ..#...
0x38,  // ..###.
0x40,  // .#....
0x30,  // ..##..
0x08,  // ....#.
0x70,  // .###..
0x10,  // ...#..
0x00,  // ......
// '%' Width:6 Height:8 Bytes:1
0x64,  // .##..#
0x64,  // .##..#
0x08,  // ....#.
0x10,  // ...#..
0x20,  // ..#...
0x4c,  // .#..##
0x4c,  // .#..##
0x00,  // ......
// '&' Width:6 Height:8 Bytes:1
0x20,  // ..#...
0x50,  // .#.#..
0x50,  // .#.#..
0x20,  // ..#...
0x54,  // .#.#.#
0x48,  // .#..#.
0x34,  // ..##.#
0x00,  // ......
// ''' Width:6 Height:8 Bytes:1
0x30,  // ..##..
0x30,  // ..##..
0x20,  // ..#...
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// '(' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x10,  // ...#..
0x00,  // ......
// ')' Width:6 Height:8 Bytes:1
0x20,  // ..#...
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x20,  // ..#...
0x00,  // ......
// '*' Width:6 Height:8 Bytes:1
0x00,  // ......
0x28,  // ..#.#.
0x38,  // ..###.
0x7c,  // .#####
0x38,  // ..###.
0x28,  // ..#.#.
0x00,  // ......
0x00,  // ......
// '+' Width:6 Height:8 Bytes:1
0x00,  // ......
0x10,  // ...#..
0x10,  // ...#..
0x7c,  // .#####
0x10,  // ...#..
0x10,  // ...#..
0x00,  // ......
0x00,  // ......
// ',' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x30,  // ..##..
0x30,  // ..##..
0x20,  // ..#...
// '-' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x7c,  // .#####
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// '.' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x30,  // ..##..
0x30,  // ..##..
0x00,  // ......
// '/' Width:6 Height:8 Bytes:1
0x00,  // ......
0x04,  // .....#
0x08,  // ....#.
0x10,  // ...#..
0x20,  // ..#...
0x40,  // .#....
0x00,  // ......
0x00,  // ......
// '0' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x4c,  // .#..##
0x54,  // .#.#.#
0x64,  // .##..#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// '1' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x30,  // ..##..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x38,  // ..###.
0x00,  // ......
// '2' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x04,  // .....#
0x18,  // ...##.
0x20,  // ..#...
0x40,  // .#....
0x7c,  // .#####
0x00,  // ......
// '3' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x04,  // .....#
0x38,  // ..###.
0x04,  // .....#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// '4' Width:6 Height:8 Bytes:1
0x08,  // ....#.
0x18,  // ...##.
0x28,  // ..#.#.
0x48,  // .#..#.
0x7c,  // .#####
0x08,  // ....#.
0x08,  // ....#.
0x00,  // ......
// '5' Width:6 Height:8 Bytes:1
0x7c,  // .#####
0x40,  // .#....
0x40,  // .#....
0x78,  // .####.
0x04,  // .....#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// '6' Width:6 Height:8 Bytes:1
0x18,  // ...##.
0x20,  // ..#...
0x40,  // .#....
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// '7' Width:6 Height:8 Bytes:1
0x7c,  // .#####
0x04,  // .....#
0x08,  // ....#.
0x10,  // ...#..
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x00,  // ......
// '8' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// '9' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x3c,  // ..####
0x04,  // .....#
0x08,  // ....#.
0x30,  // ..##..
0x00,  // ......
// ':' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x30,  // ..##..
0x30,  // ..##..
0x00,  // ......
0x30,  // ..##..
0x30,  // ..##..
0x00,  // ......
// ';' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x30,  // ..##..
0x30,  // ..##..
0x00,  // ......
0x30,  // ..##..
0x30,  // ..##..
0x20,  // ..#...
// '<' Width:6 Height:8 Bytes:1
0x08,  // ....#.
0x10,  // ...#..
0x20,  // ..#...
0x40,  // .#....
0x20,  // ..#...
0x10,  // ...#..
0x08,  // ....#.
0x00,  // ......
// '=' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x7c,  // .#####
0x00,  // ......
0x00,  // ......
0x7c,  // .#####
0x00,  // ......
0x00,  // ......
// '>' Width:6 Height:8 Bytes:1
0x20,  // ..#...
0x10,  // ...#..
0x08,  // ....#.
0x04,  // .....#
0x08,  // ....#.
0x10,  // ...#..
0x20,  // ..#...
0x00,  // ......
// '?' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x04,  // .....#
0x18,  // ...##.
0x10,  // ...#..
0x00,  // ......
0x10,  // ...#..
0x00,  // ......
// '@' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x5c,  // .#.###
0x54,  // .#.#.#
0x5c,  // .#.###
0x40,  // .#....
0x38,  // ..###.
0x00,  // ......
// 'A' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x7c,  // .#####
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'B' Width:6 Height:8 Bytes:1
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x00,  // ......
// 'C' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'D' Width:6 Height:8 Bytes:1
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x00,  // ......
// 'E' Width:6 Height:8 Bytes:1
0x7c,  // .#####
0x40,  // .#....
0x40,  // .#....
0x78,  // .####.
0x40,  // .#....
0x40,  // .#....
0x7c,  // .#####
0x00,  // ......
// 'F' Width:6 Height:8 Bytes:1
0x7c,  // .#####
0x40,  // .#....
0x40,  // .#....
0x78,  // .####.
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x00,  // ......
// 'G' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x40,  // .#....
0x5c,  // .#.###
0x44,  // .#...#
0x44,  // .#...#
0x3c,  // ..####
0x00,  // ......
// 'H' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x7c,  // .#####
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'I' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x38,  // ..###.
0x00,  // ......
// 'J' Width:6 Height:8 Bytes:1
0x04,  // .....#
0x04,  // .....#
0x04,  // .....#
0x04,  // .....#
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'K' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x48,  // .#..#.
0x50,  // .#.#..
0x60,  // .##...
0x50,  // .#.#..
0x48,  // .#..#.
0x44,  // .#...#
0x00,  // ......
// 'L' Width:6 Height:8 Bytes:1
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x7c,  // .#####
0x00,  // ......
// 'M' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x6c,  // .##.##
0x54,  // .#.#.#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'N' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x64,  // .##..#
0x54,  // .#.#.#
0x4c,  // .#..##
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'O' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'P' Width:6 Height:8 Bytes:1
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x40,  // .#....
0x40,  // .#....
0x40,  // .#....
0x00,  // ......
// 'Q' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x54,  // .#.#.#
0x48,  // .#..#.
0x34,  // ..##.#
0x00,  // ......
// 'R' Width:6 Height:8 Bytes:1
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x48,  // .#..#.
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'S' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x44,  // .#...#
0x40,  // .#....
0x38,  // ..###.
0x04,  // .....#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'T' Width:6 Height:8 Bytes:1
0x7c,  // .#####
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x00,  // ......
// 'U' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'V' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x28,  // ..#.#.
0x10,  // ...#..
0x00,  // ......
// 'W' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x44,  // .#...#
0x54,  // .#.#.#
0x54,  // .#.#.#
0x54,  // .#.#.#
0x54,  // .#.#.#
0x28,  // ..#.#.
0x00,  // ......
// 'X' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x44,  // .#...#
0x28,  // ..#.#.
0x10,  // ...#..
0x28,  // ..#.#.
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'Y' Width:6 Height:8 Bytes:1
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x28,  // ..#.#.
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x00,  // ......
// 'Z' Width:6 Height:8 Bytes:1
0x78,  // .####.
0x08,  // ....#.
0x10,  // ...#..
0x20,  // ..#...
0x40,  // .#....
0x40,  // .#....
0x78,  // .####.
0x00,  // ......
// '[' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x38,  // ..###.
0x00,  // ......
// '\' Width:6 Height:8 Bytes:1
0x00,  // ......
0x40,  // .#....
0x20,  // ..#...
0x10,  // ...#..
0x08,  // ....#.
0x04,  // .....#
0x00,  // ......
0x00,  // ......
// ']' Width:6 Height:8 Bytes:1
0x38,  // ..###.
0x08,  // ....#.
0x08,  // ....#.
0x08,  // ....#.
0x08,  // ....#.
0x08,  // ....#.
0x38,  // ..###.
0x00,  // ......
// '^' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x28,  // ..#.#.
0x44,  // .#...#
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// '_' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0xfc,  // ######
// '`' Width:6 Height:8 Bytes:1
0x30,  // ..##..
0x30,  // ..##..
0x10,  // ...#..
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// 'a' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x38,  // ..###.
0x04,  // .....#
0x3c,  // ..####
0x44,  // .#...#
0x3c,  // ..####
0x00,  // ......
// 'b' Width:6 Height:8 Bytes:1
0x40,  // .#....
0x40,  // .#....
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x00,  // ......
// 'c' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x38,  // ..###.
0x44,  // .#...#
0x40,  // .#....
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'd' Width:6 Height:8 Bytes:1
0x04,  // .....#
0x04,  // .....#
0x3c,  // ..####
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x3c,  // ..####
0x00,  // ......
// 'e' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x38,  // ..###.
0x44,  // .#...#
0x78,  // .####.
0x40,  // .#....
0x38,  // ..###.
0x00,  // ......
// 'f' Width:6 Height:8 Bytes:1
0x18,  // ...##.
0x20,  // ..#...
0x20,  // ..#...
0x78,  // .####.
0x20,  // ..#...
0x20,  // ..#...
0x20,  // ..#...
0x00,  // ......
// 'g' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x3c,  // ..####
0x44,  // .#...#
0x44,  // .#...#
0x3c,  // ..####
0x04,  // .....#
0x38,  // ..###.
// 'h' Width:6 Height:8 Bytes:1
0x40,  // .#....
0x40,  // .#....
0x70,  // .###..
0x48,  // .#..#.
0x48,  // .#..#.
0x48,  // .#..#.
0x48,  // .#..#.
0x00,  // ......
// 'i' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x00,  // ......
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x18,  // ...##.
0x00,  // ......
// 'j' Width:6 Height:8 Bytes:1
0x08,  // ....#.
0x00,  // ......
0x18,  // ...##.
0x08,  // ....#.
0x08,  // ....#.
0x08,  // ....#.
0x48,  // .#..#.
0x30,  // ..##..
// 'k' Width:6 Height:8 Bytes:1
0x40,  // .#....
0x40,  // .#....
0x48,  // .#..#.
0x50,  // .#.#..
0x60,  // .##...
0x50,  // .#.#..
0x48,  // .#..#.
0x00,  // ......
// 'l' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x18,  // ...##.
0x00,  // ......
// 'm' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x68,  // .##.#.
0x54,  // .#.#.#
0x54,  // .#.#.#
0x44,  // .#...#
0x44,  // .#...#
0x00,  // ......
// 'n' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x70,  // .###..
0x48,  // .#..#.
0x48,  // .#..#.
0x48,  // .#..#.
0x48,  // .#..#.
0x00,  // ......
// 'o' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x38,  // ..###.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x38,  // ..###.
0x00,  // ......
// 'p' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x78,  // .####.
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x78,  // .####.
0x40,  // .#....
// 'q' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x3c,  // ..####
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x3c,  // ..####
0x04,  // .....#
// 'r' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x58,  // .#.##.
0x24,  // ..#..#
0x20,  // ..#...
0x20,  // ..#...
0x70,  // .###..
0x00,  // ......
// 's' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x38,  // ..###.
0x40,  // .#....
0x38,  // ..###.
0x04,  // .....#
0x38,  // ..###.
0x00,  // ......
// 't' Width:6 Height:8 Bytes:1
0x00,  // ......
0x20,  // ..#...
0x78,  // .####.
0x20,  // ..#...
0x20,  // ..#...
0x28,  // ..#.#.
0x10,  // ...#..
0x00,  // ......
// 'u' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x48,  // .#..#.
0x48,  // .#..#.
0x48,  // .#..#.
0x58,  // .#.##.
0x28,  // ..#.#.
0x00,  // ......
// 'v' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x44,  // .#...#
0x44,  // .#...#
0x44,  // .#...#
0x28,  // ..#.#.
0x10,  // ...#..
0x00,  // ......
// 'w' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x44,  // .#...#
0x44,  // .#...#
0x54,  // .#.#.#
0x7c,  // .#####
0x28,  // ..#.#.
0x00,  // ......
// 'x' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x48,  // .#..#.
0x48,  // .#..#.
0x30,  // ..##..
0x48,  // .#..#.
0x48,  // .#..#.
0x00,  // ......
// 'y' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x48,  // .#..#.
0x48,  // .#..#.
0x48,  // .#..#.
0x38,  // ..###.
0x10,  // ...#..
0x60,  // .##...
// 'z' Width:6 Height:8 Bytes:1
0x00,  // ......
0x00,  // ......
0x78,  // .####.
0x08,  // ....#.
0x30,  // ..##..
0x40,  // .#....
0x78,  // .####.
0x00,  // ......
// '{' Width:6 Height:8 Bytes:1
0x18,  // ...##.
0x20,  // ..#...
0x20,  // ..#...
0x60,  // .##...
0x20,  // ..#...
0x20,  // ..#...
0x18,  // ...##.
0x00,  // ......
// '|' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x00,  // ......
0x10,  // ...#..
0x10,  // ...#..
0x10,  // ...#..
0x00,  // ......
// '}' Width:6 Height:8 Bytes:1
0x30,  // ..##..
0x08,  // ....#.
0x08,  // ....#.
0x0c,  // ....##
0x08,  // ....#.
0x08,  // ....#.
0x30,  // ..##..
0x00,  // ......
// '~' Width:6 Height:8 Bytes:1
0x28,  // ..#.#.
0x50,  // .#.#..
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
0x00,  // ......
// '' Width:6 Height:8 Bytes:1
0x10,  // ...#..
0x38,  // ..###.
0x6c,  // .##.##
0x44,  // .#...#
0x44,  // .#...#
0x7c,  // .#####
0x00,  // ......
0x00,  // ......
};

const unsigned char font6x8_mapping_table[] PROGMEM = {
 0, // 0 =  
 0, // 1 =  
 0, // 2 =  
 0, // 3 =  
 0, // 4 =  
 0, // 5 =  
 0, // 6 =  
 0, // 7 =  
 0, // 8 =  
 0, // 9 =  
 0, // 10 =  
 0, // 11 =  
 0, // 12 =  
 0, // 13 =  
 0, // 14 =  
 0, // 15 =  
 0, // 16 =  
 0, // 17 =  
 0, // 18 =  
 0, // 19 =  
 0, // 20 =  
 0, // 21 =  
 0, // 22 =  
 0, // 23 =  
 0, // 24 =  
 0, // 25 =  
 0, // 26 =  
 0, // 27 =  
 0, // 28 =  
 0, // 29 =  
 0, // 30 =  
 0, // 31 =  
 0, // 32 =  
 1, // 33 = !
 2, // 34 = "
 3, // 35 = #
 4, // 36 = $
 5, // 37 = %
 6, // 38 = &
 7, // 39 = '
 8, // 40 = (
 9, // 41 = )
 10, // 42 = *
 11, // 43 = +
 12, // 44 = ,
 13, // 45 = -
 14, // 46 = .
 15, // 47 = /
 16, // 48 = 0
 17, // 49 = 1
 18, // 50 = 2
 19, // 51 = 3
 20, // 52 = 4
 21, // 53 = 5
 22, // 54 = 6
 23, // 55 = 7
 24, // 56 = 8
 25, // 57 = 9
 26, // 58 = :
 27, // 59 = ;
 28, // 60 = <
 29, // 61 = =
 30, // 62 = >
 31, // 63 = ?
 32, // 64 = @
 33, // 65 = A
 34, // 66 = B
 35, // 67 = C
 36, // 68 = D
 37, // 69 = E
 38, // 70 = F
 39, // 71 = G
 40, // 72 = H
 41, // 73 = I
 42, // 74 = J
 43, // 75 = K
 44, // 76 = L
 45, // 77 = M
 46, // 78 = N
 47, // 79 = O
 48, // 80 = P
 49, // 81 = Q
 50, // 82 = R
 51, // 83 = S
 52, // 84 = T
 53, // 85 = U
 54, // 86 = V
 55, // 87 = W
 56, // 88 = X
 57, // 89 = Y
 58, // 90 = Z
 59, // 91 = [
 60, // 92 = \ .
 61, // 93 = ]
 62, // 94 = ^
 63, // 95 = _
 64, // 96 = `
 65, // 97 = a
 66, // 98 = b
 67, // 99 = c
 68, // 100 = d
 69, // 101 = e
 70, // 102 = f
 71, // 103 = g
 72, // 104 = h
 73, // 105 = i
 74, // 106 = j
 75, // 107 = k
 76, // 108 = l
 77, // 109 = m
 78, // 110 = n
 79, // 111 = o
 80, // 112 = p
 81, // 113 = q
 82, // 114 = r
 83, // 115 = s
 84, // 116 = t
 85, // 117 = u
 86, // 118 = v
 87, // 119 = w
 88, // 120 = x
 89, // 121 = y
 90, // 122 = z
 91, // 123 = {
 92, // 124 = |
 93, // 125 = }
 94, // 126 = ~
 95, // 127 = 
 0, // 128 = �
 0, // 129 = �
 0, // 130 = �
 0, // 131 = �
 0, // 132 = �
 0, // 133 = �
 0, // 134 = �
 0, // 135 = �
 0, // 136 = �
 0, // 137 = �
 0, // 138 = �
 0, // 139 = �
 0, // 140 = �
 0, // 141 = �
 0, // 142 = �
 0, // 143 = �
 0, // 144 = �
 0, // 145 = �
 0, // 146 = �
 0, // 147 = �
 0, // 148 = �
 0, // 149 = �
 0, // 150 = �
 0, // 151 = �
 0, // 152 = �
 0, // 153 = �
 0, // 154 = �
 0, // 155 = �
 0, // 156 = �
 0, // 157 = �
 0, // 158 = �
 0, // 159 = �
 0, // 160 = �
 0, // 161 = �
 0, // 162 = �
 0, // 163 = �
 0, // 164 = �
 0, // 165 = �
 0, // 166 = �
 0, // 167 = �
 0, // 168 = �
 0, // 169 = �
 0, // 170 = �
 0, // 171 = �
 0, // 172 = �
 0, // 173 = �
 0, // 174 = �
 0, // 175 = �
 0, // 176 = �
 0, // 177 = �
 0, // 178 = �
 0, // 179 = �
 0, // 180 = �
 0, // 181 = �
 0, // 182 = �
 0, // 183 = �
 0, // 184 = �
 0, // 185 = �
 0, // 186 = �
 0, // 187 = �
 0, // 188 = �
 0, // 189 = �
 0, // 190 = �
 0, // 191 = �
 0, // 192 = �
 0, // 193 = �
 0, // 194 = �
 0, // 195 = �
 0, // 196 = �
 0, // 197 = �
 0, // 198 = �
 0, // 199 = �
 0, // 200 = �
 0, // 201 = �
 0, // 202 = �
 0, // 203 = �
 0, // 204 = �
 0, // 205 = �
 0, // 206 = �
 0, // 207 = �
 0, // 208 = �
 0, // 209 = �
 0, // 210 = �
 0, // 211 = �
 0, // 212 = �
 0, // 213 = �
 0, // 214 = �
 0, // 215 = �
 0, // 216 = �
 0, // 217 = �
 0, // 218 = �
 0, // 219 = �
 0, // 220 = �
 0, // 221 = �
 0, // 222 = �
 0, // 223 = �
 0, // 224 = �
 0, // 225 = �
 0, // 226 = �
 0, // 227 = �
 0, // 228 = �
 0, // 229 = �
 0, // 230 = �
 0, // 231 = �
 0, // 232 = �
 0, // 233 = �
 0, // 234 = �
 0, // 235 = �
 0, // 236 = �
 0, // 237 = �
 0, // 238 = �
 0, // 239 = �
 0, // 240 = �
 0, // 241 = �
 0, // 242 = �
 0, // 243 = �
 0, // 244 = �
 0, // 245 = �
 0, // 246 = �
 0, // 247 = �
 0, // 248 = �
 0, // 249 = �
 0, // 250 = �
 0, // 251 = �
 0, // 252 = �
 0, // 253 = �
 0, // 254 = �
 0, // 255 = �
};

const unsigned int font6x8_offset_table[] PROGMEM = {
 0, //  
 8, // !
 16, // "
 24, // #
 32, // $
 40, // %
 48, // &
 56, // '
 64, // (
 72, // )
 80, // *
 88, // +
 96, // ,
 104, // -
 112, // .
 120, // /
 128, // 0
 136, // 1
 144, // 2
 152, // 3
 160, // 4
 168, // 5
 176, // 6
 184, // 7
 192, // 8
 200, // 9
 208, // :
 216, // ;
 224, // <
 232, // =
 240, // >
 248, // ?
 256, // @
 264, // A
 272, // B
 280, // C
 288, // D
 296, // E
 304, // F
 312, // G
 320, // H
 328, // I
 336, // J
 344, // K
 352, // L
 360, // M
 368, // N
 376, // O
 384, // P
 392, // Q
 400, // R
 408, // S
 416, // T
 424, // U
 432, // V
 440, // W
 448, // X
 456, // Y
 464, // Z
 472, // [
 480, // \ .
 488, // ]
 496, // ^
 504, // _
 512, // `
 520, // a
 528, // b
 536, // c
 544, // d
 552, // e
 560, // f
 568, // g
 576, // h
 584, // i
 592, // j
 600, // k
 608, // l
 616, // m
 624, // n
 632, // o
 640, // p
 648, // q
 656, // r
 664, // s
 672, // t
 680, // u
 688, // v
 696, // w
 704, // x
 712, // y
 720, // z
 728, // {
 736, // |
 744, // }
 752, // ~
 760, // 
};

const struct FONT_DEF font6x8 = {
   6, 	// glyph_width
   8, 	// glyph_height
   font6x8_char_table, 	// ptr to glyph_table
   font6x8_mapping_table, 	// ptr to mapping_table
   NULL,
   font6x8_offset_table
};

