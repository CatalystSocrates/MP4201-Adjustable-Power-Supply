#include "Kalman_Filter.h"

/************************ 矩阵运算辅助函数（固定维度，修正const+参数问题） ************************/
// 3x3矩阵 × 3x1矩阵 → 3x1矩阵（输入const修饰，输出无const）
static void mat_mult_3x3_3x1(const float A[3][3], const float B[3], float C[3]) {
    for (int i = 0; i < 3; i++) {
        C[i] = A[i][0]*B[0] + A[i][1]*B[1] + A[i][2]*B[2];
    }
}

// 3x3矩阵 × 3x3矩阵 → 3x3矩阵（输入const修饰，输出无const）
static void mat_mult_3x3_3x3(const float A[3][3], const float B[3][3], float C[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            C[i][j] = A[i][0]*B[0][j] + A[i][1]*B[1][j] + A[i][2]*B[2][j];
        }
    }
}

// 1x3矩阵 × 3x3矩阵 → 1x3矩阵（输入const修饰，输出无const）
static void mat_mult_1x3_3x3(const float A[3], const float B[3][3], float C[3]) {
    for (int j = 0; j < 3; j++) {
        C[j] = A[0]*B[0][j] + A[1]*B[1][j] + A[2]*B[2][j];
    }
}

// 1x3矩阵 × 3x1矩阵 → 标量（输入const修饰）
static float mat_mult_1x3_3x1(const float A[3], const float B[3]) {
    return A[0]*B[0] + A[1]*B[1] + A[2]*B[2];
}

// 3x1矩阵 × 1x3矩阵 → 3x3矩阵（输入const修饰，输出无const）
static void mat_mult_3x1_1x3(const float A[3], const float B[3], float C[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            C[i][j] = A[i] * B[j];
        }
    }
}

// 3x3矩阵转置（输入const修饰，输出无const）
static void mat_trans_3x3(const float A[3][3], float AT[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            AT[i][j] = A[j][i];
        }
    }
}

// 3x3矩阵加法（输入const修饰，输出无const）
static void mat_add_3x3(const float A[3][3], const float B[3][3], float C[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

// 3x1矩阵加法（输入const修饰，输出无const）
static void mat_add_3x1(const float A[3], const float B[3], float C[3]) {
    for (int i = 0; i < 3; i++) {
        C[i] = A[i] + B[i];
    }
}

// 3x3矩阵缩放（直接修改原数组，无const）
static void mat_scale_3x3(float A[3][3], float scale) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            A[i][j] *= scale;
        }
    }
}

// 3x1矩阵缩放（直接修改原数组，无const）
static void mat_scale_3x1(float A[3], float scale) {
    for (int i = 0; i < 3; i++) {
        A[i] *= scale;
    }
}

// 新增：3x1矩阵缩放（输出到新数组，不修改原数组，解决核心错误）
static void mat_scale_3x1_out(const float A[3], float scale, float C[3]) {
    for (int i = 0; i < 3; i++) {
        C[i] = A[i] * scale;
    }
}

// 设置3x3单位矩阵（输出无const）
static void mat_set_identity_3x3(float A[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            A[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

/************************ 卡尔曼滤波器核心函数 ************************/
// 初始化三维卡尔曼滤波器（电流采集专用）
void Kalman3D_Current_Init(Kalman3D_Current *kf, float initial_i, float initial_di, float initial_ddi, float dt) {
    // 初始化状态向量 x [电流, 电流变化率, 电流加速度]
    kf->x[0] = initial_i;
    kf->x[1] = initial_di;
    kf->x[2] = initial_ddi;

    // 初始化协方差矩阵 P (单位矩阵，初始不确定性)
    mat_set_identity_3x3(kf->P);

    // 初始化过程噪声协方差 Q (对角矩阵，电流变化的噪声)
    mat_set_identity_3x3(kf->Q);
    mat_scale_3x3(kf->Q, 0.001f);

    // 初始化状态转移矩阵 A (基于时间的线性运动模型)
    kf->A[0][0] = 1.0f; kf->A[0][1] = dt;      kf->A[0][2] = 0.5f * dt * dt;
    kf->A[1][0] = 0.0f; kf->A[1][1] = 1.0f;    kf->A[1][2] = dt;
    kf->A[2][0] = 0.0f; kf->A[2][1] = 0.0f;    kf->A[2][2] = 1.0f;

    // 初始化测量矩阵 H (仅测量电流值，对应状态向量第一个元素)
    kf->H[0] = 1.0f; kf->H[1] = 0.0f; kf->H[2] = 0.0f;

    // 测量噪声协方差（根据电流采样电路的噪声特性调整）
    kf->R = 0.1f;

    // 采样间隔
    kf->dt = dt;
}

// 三维卡尔曼滤波更新（电流采集专用）
float Kalman3D_Current_Update(Kalman3D_Current *kf, float measurement) {
    // 临时变量（避免动态分配，提升实时性）
    float temp1[3][3] = {0};  // 3x3 临时矩阵1
    float temp2[3][3] = {0};  // 3x3 临时矩阵2
    float temp3[3] = {0};     // 3x1 临时矩阵3
    float H_T[3] = {0};       // H的转置 (3x1)
    float y, S;

    // ====================== 1. 预测步骤 ======================
    // 状态预测：x_pred = A * x
    mat_mult_3x3_3x1(kf->A, kf->x, kf->x_pred);

    // 协方差预测：P_pred = A * P * A^T + Q
    mat_mult_3x3_3x3(kf->A, kf->P, temp1);        // temp1 = A * P
    mat_trans_3x3(kf->A, temp2);                   // temp2 = A^T
    mat_mult_3x3_3x3(temp1, temp2, kf->P_pred);   // P_pred = (A * P) * A^T
    mat_add_3x3(kf->P_pred, kf->Q, kf->P_pred);   // P_pred += Q

    // ====================== 2. 更新步骤 ======================
    // 创新值：y = 测量值 - H * x_pred
    y = measurement - mat_mult_1x3_3x1(kf->H, kf->x_pred);

    // 创新协方差：S = H * P_pred * H^T + R
    mat_mult_1x3_3x3(kf->H, kf->P_pred, temp3);   // temp3 = H * P_pred
    H_T[0] = kf->H[0]; H_T[1] = kf->H[1]; H_T[2] = kf->H[2]; // H转置（1x3→3x1）
    S = mat_mult_1x3_3x1(temp3, H_T) + kf->R;

    // 卡尔曼增益：K = P_pred * H^T / S
    mat_mult_3x3_3x1(kf->P_pred, H_T, kf->K);     // K = P_pred * H^T
    mat_scale_3x1(kf->K, 1.0f / S);               // K /= S

    // 状态更新：x = x_pred + K * y（改用新增的缩放函数，解决参数个数错误）
    mat_scale_3x1_out(kf->K, y, temp3);           // temp3 = K * y（不修改原K数组）
    mat_add_3x1(kf->x_pred, temp3, kf->x);        // x = x_pred + K * y

    // 协方差更新：P = (I - K * H) * P_pred
    mat_mult_3x1_1x3(kf->K, kf->H, temp1);        // temp1 = K * H
    mat_scale_3x3(temp1, -1.0f);                  // temp1 = -K * H
    mat_set_identity_3x3(temp2);                  // temp2 = 单位矩阵I
    mat_add_3x3(temp1, temp2, temp1);             // temp1 = I - K * H
    mat_mult_3x3_3x3(temp1, kf->P_pred, kf->P);   // P = (I - K * H) * P_pred

    // 返回滤波后的电流值（状态向量第一个元素）
    return kf->x[0];
}

// 电流采样滤波器实例
Kalman3D_Current kf_current_in;
Kalman3D_Current kf_current_out;
Kalman3D_Current kf_voltage_in;
Kalman3D_Current kf_voltage_out;
// 新增换行符，解决#1-D警告