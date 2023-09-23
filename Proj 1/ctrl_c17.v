module c17 (N1,N2,N3,N6,N7,N22,N23);
input N1,N2,N3,N6,N7;
output N22,N23;
wire N10,N11,N16,N19,N20,N21,N24;
nand NAND2_2 (N10, N1, N3);	//ctrl = 0.75
nand NAND2_6 (N11, N3, N6);	//ctrl = 0.75
nand NAND2_1 (N16, N2, N11);	//ctrl = 0.625
nand NAND2_3 (N19, N11, N7);	//ctrl = 0.625
nand NAND2_4 (N23, N16, N19);	//ctrl = 0.609375
nand NAND2_5 (N22, N10, N16);	//ctrl = 0.53125
xor XOR5_1 (N20, N10, N11, N16, N19, N23);	//ctrl = 0.501709
nand NAND2_7 (N24, N1, N1);	//ctrl = 0.5
endmodule