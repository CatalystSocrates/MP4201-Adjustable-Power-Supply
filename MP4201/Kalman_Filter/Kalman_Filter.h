#ifndef KALMAN_FILTER
#define KALMAN_FILTER

/************************ 三维卡尔曼滤波器（电流采集专用）************************/
/**
 * @brief 三维卡尔曼滤波器结构体（适配电流采集，无DSP库依赖）
 * @note 状态向量：[电流(A), 电流变化率(A/s), 电流加速度(A/s²)]
 */
typedef struct {
    float x[3];               // 状态向量 [电流, 电流变化率, 电流加速度] (3x1)
    float P[3][3];            // 估计协方差矩阵 (3x3)
    float Q[3][3];            // 过程噪声协方差矩阵 (3x3)
    float R;                  // 测量噪声协方差（可根据硬件特性动态调整）
    float dt;                 // 采样间隔（秒）
    float A[3][3];            // 状态转移矩阵 (3x3)
    float H[3];               // 测量矩阵 (1x3)
    float x_pred[3];          // 预测状态 (3x1)
    float P_pred[3][3];       // 预测协方差 (3x3)
    float K[3];               // 卡尔曼增益 (3x1)
} Kalman3D_Current;

/**
 * @brief 初始化三维卡尔曼滤波器（电流采集专用）
 * @param kf        滤波器实例指针（必须非NULL）
 * @param initial_i 初始电流值 (A)
 * @param initial_di 初始电流变化率 (A/s)
 * @param initial_ddi 初始电流加速度 (A/s²)
 * @param dt        采样间隔（秒，建议与实际采样周期一致）
 * @note 初始变化率/加速度若无先验值，可设为0.0f
 */
void Kalman3D_Current_Init(Kalman3D_Current *kf, float initial_i, float initial_di, float initial_ddi, float dt);

/**
 * @brief 三维卡尔曼滤波更新（电流采集专用）
 * @param kf          滤波器实例指针（必须非NULL）
 * @param measurement 电流采样值 (A)
 * @return 滤波后的电流值 (A)
 * @note 需在每次采样后调用，确保采样周期与初始化的dt一致
 */
float Kalman3D_Current_Update(Kalman3D_Current *kf, float measurement);

/**
 * @brief 全局电流滤波实例（如需多个实例，可自行定义，或删除此全局变量）
 * @note 若项目中需多通道电流滤波，建议每个通道单独定义实例，避免全局变量冲突
 */
extern Kalman3D_Current kf_current_in;
extern Kalman3D_Current kf_current_out;


#endif
