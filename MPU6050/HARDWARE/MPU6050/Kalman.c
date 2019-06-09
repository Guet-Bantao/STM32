#include "Kalman.h"
#include "math.h"


//�����ⲿ����
extern short aacx, aacy, aacz;		//���ٶȴ�����ԭʼ����
extern short gyrox, gyroy, gyroz;	//������ԭʼ����

float Accel_x;	     	//X����ٶ�ֵ�ݴ�
float Accel_y;	     	//Y����ٶ�ֵ�ݴ�
float Accel_z;	     	//Z����ٶ�ֵ�ݴ�

float Gyro_x;		    //X�������������ݴ�
float Gyro_y;    		//Y�������������ݴ�
float Gyro_z;		    //Z�������������ݴ�

float Angle_x_temp;  	//�ɼ��ٶȼ����x��б�Ƕ�
float Angle_y_temp;  	//�ɼ��ٶȼ����y��б�Ƕ�
float Angle_z_temp;		//�ɼ��ٶȼ����z��б�Ƕ�

float Angle_X_Final; 	//X������б�Ƕ�
float Angle_Y_Final; 	//Y������б�Ƕ�
float Angle_Z_Final; 	//Z������б�Ƕ�

float pitch,roll,yaw; 		//ŷ����

//����������
char  C_0 = 1;
float Q_bias_x, Q_bias_y, Q_bias_z;
float Angle_err_x, Angle_err_y, Angle_err_z;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] = { 0,0,0,0 };
float PP[2][2] = { { 1, 0 },{ 0, 1 } };


//�Ƕȼ���
void Angle_Calcu(void)
{
	//��ΧΪ2gʱ�������ϵ��16384 LSB/g
	//deg = rad*180/3.14
	float x=0, y=0, z=0;

	Accel_x = aacx; //x����ٶ�ֵ�ݴ�
	Accel_y = aacy; //y����ٶ�ֵ�ݴ�
	Accel_z = aacz; //z����ٶ�ֵ�ݴ�
	Gyro_x = gyrox;  //x��������ֵ�ݴ�
	Gyro_y = gyroy;  //y��������ֵ�ݴ�
	Gyro_z = gyroz;  //z��������ֵ�ݴ�

	//����x����ٶ�
	if (Accel_x<32768) x = Accel_x / 16384;
	else              x = 1 - (Accel_x - 49152) / 16384;

	//����y����ٶ�
	if (Accel_y<32768) y = Accel_y / 16384;
	else              y = 1 - (Accel_y - 49152) / 16384;

	//����z����ٶ�
	if (Accel_z<32768) z = Accel_z / 16384;
	else              z = (Accel_z - 49152) / 16384;

	//�ü��ٶȼ����������ˮƽ������ϵ֮��ļн�
	Angle_x_temp = (atan2(z , y)) * 180 / Pi;//atan2(z , y)
	Angle_y_temp = (atan2(x , z)) * 180 / Pi;
	Angle_z_temp = (atan2(y , x)) * 180 / Pi;
//	Angle_accX=(atan(sqrt(Ax*Ax+Ay*Ay)/Az))*1800/3.14;
//		Angle_accY=(atan(Ax/sqrt(Ay*Ay+Az*Az)))*1800/3.14;
//		Angle_accZ=(atan(Ay/axrt(Ax*Ax+Az*Az)))*1800/3.14;

	//�Ƕȵ�������
	if (Accel_y<32768) Angle_y_temp = -Angle_y_temp;//
	if (Accel_y>32768) Angle_y_temp = +Angle_y_temp;//
	if (Accel_x<32768) Angle_x_temp = +Angle_x_temp;
	if (Accel_x>32768) Angle_x_temp = -Angle_x_temp;
	if (Accel_z<32768) Angle_z_temp = +Angle_z_temp;
	if (Accel_z>32768) Angle_z_temp = -Angle_z_temp;
	
	//���ٶ�
	//��ǰ�˶�
	if (Gyro_x<32768) Gyro_x = -(Gyro_x / 16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if (Gyro_x>32768) Gyro_x = +(65535 - Gyro_x) / 16.4;
	//��ǰ�˶�
	if (Gyro_y<32768) Gyro_y = -(Gyro_y / 16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if (Gyro_y>32768) Gyro_y = +(65535 - Gyro_y) / 16.4;
	//��ǰ�˶�
	if (Gyro_z<32768) Gyro_z = -(Gyro_z / 16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if (Gyro_z>32768) Gyro_z = +(65535 - Gyro_z) / 16.4;

	Kalman_Filter_X(Angle_x_temp, Gyro_x);  //�������˲�����X���
	Kalman_Filter_Y(Angle_y_temp, Gyro_y);  //�������˲�����Y���
	Kalman_Filter_Z(Angle_z_temp, Gyro_z);  //�������˲�����Z���
}

void Kalman_Filter_X(float Accel, float Gyro) //����������
{
	Angle_X_Final += (Gyro - Q_bias_x) * dt; //�������

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_x = Accel - Angle_X_Final;	//zk-�������

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_X_Final += K_0 * Angle_err_x;	 //�������
	Q_bias_x += K_1 * Angle_err_x;	 //�������
	Gyro_x = Gyro - Q_bias_x;	 //���ֵ(�������)��΢��=���ٶ�
	
	roll=Angle_X_Final;
	if(roll<-90)
	{
		roll=-roll;
		roll=180-(roll-90.0)+90;
	}
}

void Kalman_Filter_Y(float Accel, float Gyro) //����������		
{
	Angle_Y_Final += (Gyro - Q_bias_y) * dt; //�������

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_y = Accel - Angle_Y_Final;	//zk-�������

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Y_Final += K_0 * Angle_err_y;	 //�������
	Q_bias_y += K_1 * Angle_err_y;	 //�������
	Gyro_y = Gyro - Q_bias_y;	 //���ֵ(�������)��΢��=���ٶ�
	
	pitch=Angle_Y_Final;
	if(pitch>90)
	{
		pitch=180-pitch;
	}
	if(pitch<-90)
	{
		pitch=-180-pitch;
	}
}

void Kalman_Filter_Z(float Accel, float Gyro) //����������
{
	Angle_Z_Final += (Gyro - Q_bias_z) * dt; //�������

	Pdot[0] = Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1] = -PP[1][1];
	Pdot[2] = -PP[1][1];
	Pdot[3] = Q_gyro;

	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;

	Angle_err_z = Accel - Angle_Z_Final;	//zk-�������

	PCt_0 = C_0 * PP[0][0];
	PCt_1 = C_0 * PP[1][0];

	E = R_angle + C_0 * PCt_0;

	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;

	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];

	PP[0][0] -= K_0 * t_0;		 //����������Э����
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;

	Angle_Z_Final += K_0 * Angle_err_z;	 //�������
	Q_bias_z += K_1 * Angle_err_z;	 //�������
	Gyro_z = Gyro - Q_bias_z;	 //���ֵ(�������)��΢��=���ٶ�
	
	yaw=Angle_Z_Final;
}


