#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// COLOR                    0   xyzw        0     NONE   float   xyzw
// TEXCOORD                 0   xy          1     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c0         cb0             1         1  ( FLT, FLT, FLT, FLT)
//
//
// Sampler/Resource to DX9 shader sampler mappings:
//
// Target Sampler Source Sampler  Source Resource
// -------------- --------------- ----------------
// s0             s0              t0               
//
//
// Level9 shader bytecode:
//
    ps_2_0
    dcl t0  // pin<0,1,2,3>
    dcl t1.xy  // pin<4,5>
    dcl_2d s0

#line 100 "C:\Users\Stan Gerard Palisoc\Desktop\MAGE\External\DirectXTK\src\Shaders\AlphaTestEffect.fx"
    texld r0, t1, s0
    mad r1.w, r0.w, t0.w, -c0.x
    mul r0, r0, t0  // ::color<0,1,2,3>
    mov oC0, r0  // ::PSAlphaTestLtGtNoFog<0,1,2,3>
    cmp r0, r1.w, c0.w, c0.z
    texkill r0

// approximately 6 instruction slots used (1 texture, 5 arithmetic)
ps_4_0
dcl_constantbuffer CB0[2], immediateIndexed
dcl_sampler s0, mode_default
dcl_resource_texture2d (float,float,float,float) t0
dcl_input_ps linear v0.xyzw
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 2
sample r0.xyzw, v1.xyxx, t0.xyzw, s0
mul r0.xyzw, r0.xyzw, v0.xyzw
lt r1.x, r0.w, cb0[1].x
mov o0.xyzw, r0.xyzw
movc r0.x, r1.x, cb0[1].z, cb0[1].w
lt r0.x, r0.x, l(0.000000)
discard_nz r0.x
ret 
// Approximately 0 instruction slots used
#endif

const BYTE AlphaTestEffect_PSAlphaTestLtGtNoFog[] =
{
     68,  88,  66,  67, 218,  28, 
    148, 124, 141, 176, 128,  41, 
    124,  24,  45, 249,  50,  14, 
     11,  85,   1,   0,   0,   0, 
    160,   4,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
    232,   2,   0,   0,  28,   4, 
      0,   0, 108,   4,   0,   0, 
     65, 111, 110,  57, 176,   2, 
      0,   0, 176,   2,   0,   0, 
      0,   2, 255, 255, 124,   2, 
      0,   0,  52,   0,   0,   0, 
      1,   0,  40,   0,   0,   0, 
     52,   0,   0,   0,  52,   0, 
      1,   0,  36,   0,   0,   0, 
     52,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   2, 255, 255, 254, 255, 
    124,   0,  68,  66,  85,  71, 
     40,   0,   0,   0, 196,   1, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0, 132,   0, 
      0,   0,   9,   0,   0,   0, 
    136,   0,   0,   0,   3,   0, 
      0,   0, 136,   1,   0,   0, 
    208,   0,   0,   0,  67,  58, 
     92,  85, 115, 101, 114, 115, 
     92,  83, 116,  97, 110,  32, 
     71, 101, 114,  97, 114, 100, 
     32,  80,  97, 108, 105, 115, 
    111,  99,  92,  68, 101, 115, 
    107, 116, 111, 112,  92,  77, 
     65,  71,  69,  92,  69, 120, 
    116, 101, 114, 110,  97, 108, 
     92,  68, 105, 114, 101,  99, 
    116,  88,  84,  75,  92, 115, 
    114,  99,  92,  83, 104,  97, 
    100, 101, 114, 115,  92,  65, 
    108, 112, 104,  97,  84, 101, 
    115, 116,  69, 102, 102, 101, 
     99, 116,  46, 102, 120,   0, 
     40,   0,   0,   0,   0,   0, 
    255, 255, 248,   1,   0,   0, 
      0,   0, 255, 255,   4,   2, 
      0,   0,   0,   0, 255, 255, 
     16,   2,   0,   0, 100,   0, 
      0,   0,  28,   2,   0,   0, 
    102,   0,   0,   0,  44,   2, 
      0,   0, 100,   0,   0,   0, 
     64,   2,   0,   0, 100,   0, 
      0,   0,  80,   2,   0,   0, 
    102,   0,   0,   0,  92,   2, 
      0,   0, 102,   0,   0,   0, 
    112,   2,   0,   0,  80,  83, 
     65, 108, 112, 104,  97,  84, 
    101, 115, 116,  76, 116,  71, 
    116,  78, 111,  70, 111, 103, 
      0, 171, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   6,   0,   0,   0, 
      0,   0,   1,   0,   2,   0, 
      3,   0,  99, 111, 108, 111, 
    114,   0, 171, 171,   1,   0, 
      3,   0,   1,   0,   4,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
      0,   0,   1,   0,   2,   0, 
      3,   0, 112, 105, 110,   0, 
     68, 105, 102, 102, 117, 115, 
    101,   0,  84, 101, 120,  67, 
    111, 111, 114, 100,   0, 171, 
    171, 171,   1,   0,   3,   0, 
      1,   0,   2,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     44,   1,   0,   0,  12,   1, 
      0,   0,  52,   1,   0,   0, 
     64,   1,   0,   0,   5,   0, 
      0,   0,   1,   0,   6,   0, 
      1,   0,   2,   0,  80,   1, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   2,   0, 
      3,   0,   1,   0,   0,   0, 
      4,   0,   5,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    208,   0,   0,   0, 232,   0, 
      0,   0,   1,   0,   0,   0, 
    248,   0,   0,   0,   0,   0, 
      0,   0,   4,   1,   0,   0, 
     12,   1,   0,   0,   1,   0, 
      0,   0,  28,   1,   0,   0, 
    208,   0,   0,   0,  40,   1, 
      0,   0,  96,   1,   0,   0, 
      2,   0,   0,   0, 112,   1, 
      0,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  49,  48,  46,  49,   0, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   0,   0,  15, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 128,   1,   0,   3, 176, 
     31,   0,   0,   2,   0,   0, 
      0, 144,   0,   8,  15, 160, 
     66,   0,   0,   3,   0,   0, 
     15, 128,   1,   0, 228, 176, 
      0,   8, 228, 160,   4,   0, 
      0,   4,   1,   0,   8, 128, 
      0,   0, 255, 128,   0,   0, 
    255, 176,   0,   0,   0, 161, 
      5,   0,   0,   3,   0,   0, 
     15, 128,   0,   0, 228, 128, 
      0,   0, 228, 176,   1,   0, 
      0,   2,   0,   8,  15, 128, 
      0,   0, 228, 128,  88,   0, 
      0,   4,   0,   0,  15, 128, 
      1,   0, 255, 128,   0,   0, 
    255, 160,   0,   0, 170, 160, 
     65,   0,   0,   1,   0,   0, 
     15, 128, 255, 255,   0,   0, 
     83,  72,  68,  82,  44,   1, 
      0,   0,  64,   0,   0,   0, 
     75,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  90,   0,   0,   3, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  88,  24,   0,   4, 
      0, 112,  16,   0,   0,   0, 
      0,   0,  85,  85,   0,   0, 
     98,  16,   0,   3, 242,  16, 
     16,   0,   0,   0,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  69,   0,   0,   9, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  16,  16,   0, 
      1,   0,   0,   0,  70, 126, 
     16,   0,   0,   0,   0,   0, 
      0,  96,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70,  30, 
     16,   0,   0,   0,   0,   0, 
     49,   0,   0,   8,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,  10, 128,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  55,   0, 
      0,  11,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     42, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     58, 128,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     49,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,   0,  13,   0, 
      4,   3,  10,   0,  16,   0, 
      0,   0,   0,   0,  62,   0, 
      0,   1,  73,  83,  71,  78, 
     72,   0,   0,   0,   2,   0, 
      0,   0,   8,   0,   0,   0, 
     56,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,  15,   0,   0, 
     62,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   3,   3,   0,   0, 
     67,  79,  76,  79,  82,   0, 
     84,  69,  88,  67,  79,  79, 
     82,  68,   0, 171,  79,  83, 
     71,  78,  44,   0,   0,   0, 
      1,   0,   0,   0,   8,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  83,  86,  95,  84, 
     97, 114, 103, 101, 116,   0, 
    171, 171
};