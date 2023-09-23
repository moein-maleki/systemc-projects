module c17 (N1,N2,N3,N6,N7,N22,N23);
input N1,N2,N3,N6,N7;
output N22,N23;
wire N10,N11,N16,N19,N20,N21,N24;
nand NAND2_2 (N10, N1, N3);	//level = 1
nand NAND2_6 (N11, N3, N6);	//level = 1
nand NAND2_7 (N24, N1, N1);	//level = 1
nand NAND2_1 (N16, N2, N11);	//level = 2
nand NAND2_3 (N19, N11, N7);	//level = 2
nand NAND2_4 (N23, N16, N19);	//level = 3
nand NAND2_5 (N22, N10, N16);	//level = 3
xor XOR5_1 (N20, N10, N11, N16, N19, N23);	//level = 4
endmodule