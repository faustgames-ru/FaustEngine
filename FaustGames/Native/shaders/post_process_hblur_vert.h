/* Generated by bin2c, do not edit manually */

/* Contents of file shader_post_process_hblur.vert */
const long int shader_post_process_hblur_vert_size = 1223;
const unsigned char shader_post_process_hblur_vert[1223] = {
    0x23, 0x69, 0x66, 0x64, 0x65, 0x66, 0x20, 0x47, 0x4C, 0x5F, 0x45, 0x53, 0x0D, 0x0A, 0x70, 0x72,
    0x65, 0x63, 0x69, 0x73, 0x69, 0x6F, 0x6E, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x66, 0x6C,
    0x6F, 0x61, 0x74, 0x3B, 0x0D, 0x0A, 0x70, 0x72, 0x65, 0x63, 0x69, 0x73, 0x69, 0x6F, 0x6E, 0x20,
    0x6D, 0x65, 0x64, 0x69, 0x75, 0x6D, 0x70, 0x20, 0x69, 0x6E, 0x74, 0x3B, 0x0D, 0x0A, 0x23, 0x65,
    0x6E, 0x64, 0x69, 0x66, 0x0D, 0x0A, 0x0D, 0x0A, 0x75, 0x6E, 0x69, 0x66, 0x6F, 0x72, 0x6D, 0x20,
    0x76, 0x65, 0x63, 0x32, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x3B, 0x0D,
    0x0A, 0x0D, 0x0A, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63,
    0x34, 0x20, 0x70, 0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x3B, 0x0D, 0x0A, 0x61, 0x74, 0x74,
    0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x74, 0x65, 0x78, 0x74,
    0x75, 0x72, 0x65, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x3B, 0x0D, 0x0A, 0x20, 0x0D, 0x0A, 0x76,
    0x61, 0x72, 0x79, 0x69, 0x6E, 0x67, 0x20, 0x76, 0x65, 0x63, 0x32, 0x20, 0x5F, 0x74, 0x65, 0x78,
    0x43, 0x6F, 0x6F, 0x72, 0x64, 0x3B, 0x0D, 0x0A, 0x76, 0x61, 0x72, 0x79, 0x69, 0x6E, 0x67, 0x20,
    0x76, 0x65, 0x63, 0x32, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F,
    0x72, 0x64, 0x73, 0x5B, 0x31, 0x34, 0x5D, 0x3B, 0x0D, 0x0A, 0x20, 0x0D, 0x0A, 0x76, 0x6F, 0x69,
    0x64, 0x20, 0x6D, 0x61, 0x69, 0x6E, 0x28, 0x29, 0x0D, 0x0A, 0x7B, 0x0D, 0x0A, 0x20, 0x20, 0x20,
    0x20, 0x67, 0x6C, 0x5F, 0x50, 0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x3D, 0x20, 0x70,
    0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x74,
    0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x3D, 0x20, 0x74, 0x65, 0x78, 0x74, 0x75, 0x72,
    0x65, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62,
    0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20, 0x30, 0x5D,
    0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2D, 0x20, 0x70,
    0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28,
    0x37, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20,
    0x31, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2D,
    0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63,
    0x32, 0x28, 0x36, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20,
    0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73,
    0x5B, 0x20, 0x32, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64,
    0x20, 0x2D, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76,
    0x65, 0x63, 0x32, 0x28, 0x35, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A,
    0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72,
    0x64, 0x73, 0x5B, 0x20, 0x33, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F,
    0x72, 0x64, 0x20, 0x2D, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A,
    0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x34, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B,
    0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F,
    0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20, 0x34, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43,
    0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2D, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65,
    0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x33, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30,
    0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78,
    0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20, 0x35, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65,
    0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2D, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69,
    0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x32, 0x2E, 0x30, 0x2C, 0x20, 0x30,
    0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54,
    0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20, 0x36, 0x5D, 0x20, 0x3D, 0x20, 0x5F,
    0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2D, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C,
    0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x31, 0x2E, 0x30, 0x2C,
    0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75,
    0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20, 0x37, 0x5D, 0x20, 0x3D,
    0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2B, 0x20, 0x70, 0x69, 0x78,
    0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x31, 0x2E,
    0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62,
    0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20, 0x38, 0x5D,
    0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2B, 0x20, 0x70,
    0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28,
    0x32, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x20,
    0x39, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2B,
    0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63,
    0x32, 0x28, 0x33, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20,
    0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73,
    0x5B, 0x31, 0x30, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64,
    0x20, 0x2B, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76,
    0x65, 0x63, 0x32, 0x28, 0x34, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A,
    0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F, 0x6F, 0x72,
    0x64, 0x73, 0x5B, 0x31, 0x31, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43, 0x6F, 0x6F,
    0x72, 0x64, 0x20, 0x2B, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65, 0x20, 0x2A,
    0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x35, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30, 0x29, 0x3B,
    0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78, 0x43, 0x6F,
    0x6F, 0x72, 0x64, 0x73, 0x5B, 0x31, 0x32, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65, 0x78, 0x43,
    0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2B, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69, 0x7A, 0x65,
    0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x36, 0x2E, 0x30, 0x2C, 0x20, 0x30, 0x2E, 0x30,
    0x29, 0x3B, 0x0D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x5F, 0x62, 0x6C, 0x75, 0x72, 0x54, 0x65, 0x78,
    0x43, 0x6F, 0x6F, 0x72, 0x64, 0x73, 0x5B, 0x31, 0x33, 0x5D, 0x20, 0x3D, 0x20, 0x5F, 0x74, 0x65,
    0x78, 0x43, 0x6F, 0x6F, 0x72, 0x64, 0x20, 0x2B, 0x20, 0x70, 0x69, 0x78, 0x65, 0x6C, 0x53, 0x69,
    0x7A, 0x65, 0x20, 0x2A, 0x20, 0x76, 0x65, 0x63, 0x32, 0x28, 0x37, 0x2E, 0x30, 0x2C, 0x20, 0x30,
    0x2E, 0x30, 0x29, 0x3B, 0x0D, 0x0A, 0x7D
};
