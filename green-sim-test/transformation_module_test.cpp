#include "pch.h"


#include "transformation-module.h"

static void MatrixMultiply(float A[4][4], float B[4][4], float C[4][4]);

TEST(TransformationModuleTest, ComputerCorrectMatrix) {
	// FUUUUCK GOTTA IMPLEMENT, not yet a fan of testing :(, hopefully I thank myself for it

    float scale_x = 1.0;
    float scale_y = 0.1;
    float scale_z = 3.9;

    float x_d = 5;
    float y_d = -3;
    float z_d = 0;

    float x_rot = 0;
    float y_rot = 0.78539;
    float z_rot = -0.3926;


    float scale_matrix[4][4] = {
                        scale_x, 0.0, 0.0, 0.1,
                        0.0, scale_y, 0.0, 0.1,
                        0.0, 0.0, scale_z, 0.1,
                        0.0, 0.0, 0.0, 1.0 };


    float trans_matrix[4][4] = {
                        1.0, 0.0, 0.0, x_d,
                        0.0, 1.0, 0.0, y_d,
                        0.0, 0.0, 1.0, z_d,
                        0.0, 0.0, 0.0, 1.0 };

    float z_rot_matrix[4][4] = {
                        cosf(z_rot), -sinf(z_rot), 0.0, 0.0,
                        sinf(z_rot), cosf(z_rot), 0.0, 0.0,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0
    };
    float y_rot_matrix[4][4] = {
                        cosf(y_rot), 0.0, -sinf(y_rot), 0.0,
                        0.0, 1.0, 0.0, 0.0,
                        sinf(y_rot), 0.0, cosf(y_rot), 0.0,
                        0.0, 0.0, 0.0, 1.0
    };
    float x_rot_matrix[4][4] = {
                       1.0, 0.0, 0.0, 0.0,
                       0.0, cosf(x_rot), -sinf(x_rot), 0.0,
                       0.0, sinf(x_rot), cosf(x_rot), 0.0,
                       0.0, 0.0, 0.0, 1.0
    };
    TransformationModule TM = TransformationModule(glm::vec3(x_d, y_d, z_d), glm::vec3(x_rot, y_rot, z_rot), glm::vec3(scale_x, scale_y, scale_z));

    glm::mat4 RESULT_M = TM.GetFinalTransformMat();

    float STEPA[4][4] = { 0,0,0,0 ,0,0,0,0 ,0,0,0,0 ,0,0,0,0 };
    float STEPB[4][4] = { 0,0,0,0 ,0,0,0,0 ,0,0,0,0 ,0,0,0,0 };
    float STEPC[4][4] = { 0,0,0,0 ,0,0,0,0 ,0,0,0,0 ,0,0,0,0 };
    float TRUE_M[4][4] = { 0,0,0,0 ,0,0,0,0 ,0,0,0,0 ,0,0,0,0 };

    MatrixMultiply(scale_matrix, x_rot_matrix, STEPA);
    MatrixMultiply(STEPA, y_rot_matrix, STEPB);
    MatrixMultiply(STEPB, z_rot_matrix, STEPC);
    MatrixMultiply(STEPC, trans_matrix, TRUE_M);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(TRUE_M[i][j], RESULT_M[i][j]) << "Mismatch at index: " << i << " " << j << std::endl;
        }
    }
}

TEST(MatrixMultiplyTest, ComputeCorrectMatrix) {

    float A[4][4] = {1.0, 2.0, 3.0, 4.0,
                    5.0, 6.0, 7.0, 8.0,
                    9.0, 10.0, 11.0, 12.0,
                    13.0, 14.0, 15.0, 16.0 };

    float B[4][4] = {1, 3, 5, 7,
                    9, 2, 4, 6,
                    8, 10, 12, 14,
                    11, 13, 15, 16 };

    float TRUE_M[4][4] = {87, 89, 109, 125,
                        203, 201, 253, 297,
                        319, 313, 397, 469,
                        435, 425, 541, 641 };

    float RESULT_M[4][4] = {0,0,0,0 ,0,0,0,0 ,0,0,0,0 ,0,0,0,0};
    
    MatrixMultiply(A, B, RESULT_M);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            EXPECT_EQ(TRUE_M[i][j], RESULT_M[i][j]) << "Mismatch at index: " << i << " " << j << std::endl;
        }
    }

}

static void MatrixMultiply(float A[4][4], float B[4][4], float C[4][4]) {

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            C[i][j] = (A[i][0]*B[0][j]) + (A[i][1] * B[1][j]) + (A[i][2] * B[2][j]) + (A[i][3] * B[3][j]);
        }
    }
}