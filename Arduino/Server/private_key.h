#ifndef PRIVATE_KEY_H_
#define PRIVATE_KEY_H_
unsigned char example_key_DER[] = {
  0x30, 0x82, 0x02, 0x5d, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xe1,
  0x3d, 0x98, 0x21, 0xfd, 0x48, 0x39, 0x3d, 0xc6, 0x2a, 0xe9, 0xb8, 0x64,
  0x34, 0x38, 0xea, 0x35, 0x80, 0xd8, 0x53, 0x7e, 0xc1, 0x15, 0x44, 0x2f,
  0xb0, 0x74, 0xa0, 0x9a, 0x4c, 0x99, 0xba, 0x56, 0x5c, 0x67, 0x06, 0x73,
  0xe2, 0xa8, 0x91, 0x23, 0xf1, 0xdd, 0x7c, 0x77, 0x10, 0x93, 0xbd, 0x84,
  0x6f, 0xd5, 0xe9, 0xe9, 0x6f, 0xcd, 0xa3, 0x3a, 0x2c, 0xd3, 0xac, 0x1a,
  0xcf, 0x7b, 0x8c, 0x46, 0xe1, 0x1c, 0x97, 0x31, 0xdf, 0x6a, 0x3d, 0xb6,
  0x65, 0xa3, 0x4a, 0x05, 0x83, 0xa7, 0xc4, 0xd6, 0x07, 0x3b, 0xa5, 0xfb,
  0x95, 0xaa, 0x6e, 0xba, 0xd5, 0x0f, 0xad, 0x0d, 0x1d, 0x7a, 0x17, 0x9b,
  0xa4, 0x2e, 0xc3, 0x0e, 0xc8, 0x41, 0x19, 0xe3, 0x03, 0x2e, 0x07, 0x68,
  0x27, 0xcd, 0x60, 0x86, 0xb3, 0x67, 0xfa, 0xa2, 0x31, 0x5b, 0xa6, 0xb5,
  0xe3, 0x65, 0x14, 0x67, 0x4e, 0xea, 0x7d, 0x02, 0x03, 0x01, 0x00, 0x01,
  0x02, 0x81, 0x80, 0x42, 0x81, 0xa4, 0x65, 0x0c, 0xdb, 0x81, 0x83, 0x5a,
  0xfb, 0x1a, 0xcd, 0xc6, 0x50, 0xa1, 0x29, 0x8e, 0xa8, 0xbf, 0xb7, 0x4f,
  0xab, 0x05, 0x73, 0x7f, 0xbd, 0x53, 0x82, 0x84, 0x6c, 0xef, 0xcb, 0x54,
  0x6a, 0xae, 0x44, 0xfa, 0x7c, 0xc7, 0x97, 0x2b, 0x8c, 0xe4, 0x00, 0x37,
  0x4b, 0xde, 0xc3, 0xcb, 0xd2, 0x15, 0xd0, 0x9e, 0x8b, 0xd3, 0xa5, 0x49,
  0xad, 0x28, 0xc7, 0x1d, 0x97, 0x7b, 0xfb, 0xee, 0xb4, 0x74, 0x78, 0xfa,
  0x93, 0x93, 0xc8, 0xdd, 0xe2, 0x67, 0x95, 0x20, 0xfa, 0x5e, 0xa7, 0xd5,
  0xee, 0x7d, 0x26, 0x58, 0x02, 0xd3, 0x8a, 0xc8, 0xec, 0x4b, 0xc4, 0x59,
  0xde, 0x3d, 0x15, 0x47, 0xd9, 0x8c, 0xc8, 0xec, 0x9e, 0x70, 0x30, 0x74,
  0xec, 0x89, 0xa4, 0x99, 0x2f, 0x07, 0xeb, 0xf3, 0x63, 0x31, 0xbc, 0xb2,
  0x66, 0x8d, 0x37, 0xe2, 0x6b, 0xc9, 0x05, 0x84, 0x62, 0xaa, 0xa5, 0x02,
  0x41, 0x00, 0xf3, 0x28, 0x30, 0x78, 0x59, 0x46, 0x68, 0x96, 0xb2, 0xff,
  0x7b, 0xac, 0xe3, 0x95, 0xb8, 0x61, 0x23, 0xa1, 0xf1, 0xa5, 0xb4, 0x93,
  0xe1, 0x54, 0x14, 0xad, 0x89, 0x41, 0x5a, 0x1a, 0xb9, 0x5b, 0x02, 0xd9,
  0x2e, 0x19, 0xe0, 0x58, 0x83, 0x11, 0x6a, 0xda, 0xdd, 0xf8, 0x3a, 0x60,
  0x9a, 0xf8, 0x5d, 0x2e, 0x0b, 0xdc, 0x69, 0x82, 0xf8, 0x78, 0xd3, 0xb8,
  0x18, 0xae, 0xe3, 0x20, 0x71, 0x7f, 0x02, 0x41, 0x00, 0xed, 0x23, 0x26,
  0xb5, 0x10, 0x64, 0x37, 0x80, 0xc2, 0x30, 0x33, 0x13, 0x97, 0xea, 0x35,
  0x94, 0xd2, 0x9a, 0xcc, 0xeb, 0x9c, 0x70, 0x81, 0xe5, 0x5e, 0x58, 0x40,
  0xae, 0x38, 0x90, 0xdc, 0xd0, 0xcc, 0x82, 0x2e, 0x1f, 0xa8, 0x56, 0xf0,
  0x9f, 0x8f, 0x41, 0x81, 0xcc, 0xb4, 0x34, 0xb1, 0xc0, 0x95, 0x2a, 0x6e,
  0xeb, 0x16, 0xa0, 0x2f, 0x27, 0xe8, 0x9a, 0x5a, 0x3a, 0xfa, 0x91, 0x6a,
  0x03, 0x02, 0x41, 0x00, 0xae, 0x93, 0xc3, 0x41, 0x14, 0xef, 0xbd, 0x35,
  0xbd, 0x6c, 0x3a, 0x92, 0x65, 0x8f, 0xe0, 0x22, 0x36, 0x20, 0x16, 0x4b,
  0xdf, 0x7b, 0x71, 0x03, 0x1a, 0x19, 0x81, 0xfb, 0x1e, 0xc3, 0x23, 0x23,
  0x59, 0xc7, 0x2e, 0x17, 0x3c, 0x00, 0xb0, 0x91, 0xb7, 0xc3, 0x4e, 0x2c,
  0xe3, 0x54, 0xea, 0xce, 0xb8, 0xaf, 0xcf, 0xaa, 0x3f, 0x42, 0xde, 0x3c,
  0x79, 0xdd, 0xaa, 0x33, 0x1e, 0x34, 0xf3, 0xe7, 0x02, 0x41, 0x00, 0x85,
  0xbf, 0xe1, 0xc1, 0x16, 0x31, 0xb0, 0x2c, 0x6b, 0x5a, 0x8c, 0x34, 0x6c,
  0xe6, 0xa8, 0xb9, 0xfb, 0xdf, 0x5b, 0xdd, 0x38, 0x2b, 0x2c, 0xd8, 0x95,
  0xd1, 0xe9, 0x09, 0x5a, 0xd6, 0x39, 0xf5, 0x7a, 0xa3, 0xa2, 0x28, 0x30,
  0xe6, 0x78, 0x69, 0x46, 0x7f, 0x88, 0x1a, 0x51, 0x39, 0xb7, 0x58, 0x0b,
  0xca, 0x17, 0x2d, 0x79, 0x61, 0x77, 0xb7, 0x66, 0xb6, 0x48, 0x56, 0x23,
  0xa5, 0x88, 0xc9, 0x02, 0x40, 0x2f, 0xd4, 0x23, 0x37, 0x5a, 0xc4, 0x45,
  0xf4, 0x71, 0x87, 0xcd, 0xc8, 0xe7, 0xf7, 0xd1, 0xeb, 0x42, 0x66, 0x66,
  0x87, 0x64, 0xd0, 0x67, 0xb7, 0x73, 0xc3, 0x0f, 0xbc, 0xc1, 0xb2, 0xd7,
  0xa0, 0x39, 0xa5, 0x6d, 0x9c, 0x1f, 0x55, 0x38, 0x3a, 0x9a, 0xfd, 0x02,
  0xaf, 0x15, 0x2c, 0xa0, 0xb6, 0xdb, 0x5d, 0x41, 0x5f, 0x5b, 0x47, 0x51,
  0xe9, 0xf1, 0x9c, 0xec, 0xbb, 0xcc, 0x67, 0xb7, 0x7b
};
unsigned int example_key_DER_len = 609;
#endif
