using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyoVisualizedApp
{
    public class Sample
    {
        public float roll { get; private set; }
        public float pitch { get; private set; }
        public float yaw { get; private set; }
        public float gyro_x { get; private set; }
        public float gyro_y { get; private set; }
        public float gyro_z { get; private set; }
        public float accel_x { get; private set; }
        public float accel_y { get; private set; }
        public float accel_z { get; private set; }
        public int EMG0 { get; private set; }
        public int EMG1 { get; private set; }
        public int EMG2 { get; private set; }
        public int EMG3 { get; private set; }
        public int EMG4 { get; private set; }
        public int EMG5 { get; private set; }
        public int EMG6 { get; private set; }
        public int EMG7 { get; private set; }
        public int stepDetect { get; private set; }
        public int muscleTension { get; private set; }
        public int time { get; private set; }
        
        //constructor without EMG
        public Sample(float roll, float pitch, float yaw, float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z, int time)
        {
            this.roll = roll;
            this.pitch = pitch;
            this.yaw = yaw;
            this.gyro_x = gyro_x;
            this.gyro_y = gyro_y;
            this.gyro_z = gyro_z;
            this.accel_x = accel_x;
            this.accel_y = accel_y;
            this.accel_z = accel_z;
            this.time = time;
        }
        //Constructor with EMG
        public Sample(float roll, float pitch, float yaw, float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z, int EMG0, int EMG1, int EMG2, int EMG3, int EMG4, int EMG5, int EMG6, int EMG7, int time)
        {
            this.roll = roll;
            this.pitch = pitch;
            this.yaw = yaw;
            this.gyro_x = gyro_x;
            this.gyro_y = gyro_y;
            this.gyro_z = gyro_z;
            this.accel_x = accel_x;
            this.accel_y = accel_y;
            this.accel_z = accel_z;
            this.EMG0 = EMG0;
            this.EMG1 = EMG1;
            this.EMG2 = EMG2;
            this.EMG3 = EMG3;
            this.EMG4 = EMG4;
            this.EMG5 = EMG5;
            this.EMG6 = EMG6;
            this.EMG7 = EMG7;
            this.time = time;
        }
        //Constructor with additional measured data
        public Sample(float roll, float pitch, float yaw, float gyro_x, float gyro_y, float gyro_z, float accel_x, float accel_y, float accel_z, int EMG0, int EMG1, int EMG2, int EMG3, int EMG4, int EMG5, int EMG6, int EMG7, int stepDetect, int muscleTension, int time)
        {
            this.roll = roll;
            this.pitch = pitch;
            this.yaw = yaw;
            this.gyro_x = gyro_x;
            this.gyro_y = gyro_y;
            this.gyro_z = gyro_z;
            this.accel_x = accel_x;
            this.accel_y = accel_y;
            this.accel_z = accel_z;
            this.EMG0 = EMG0;
            this.EMG1 = EMG1;
            this.EMG2 = EMG2;
            this.EMG3 = EMG3;
            this.EMG4 = EMG4;
            this.EMG5 = EMG5;
            this.EMG6 = EMG6;
            this.EMG7 = EMG7;
            this.stepDetect = stepDetect;
            this.muscleTension = muscleTension;
            this.time = time;
        }
        public virtual string ToString()
        {
            return  roll.ToString()     + ", " +
                    pitch.ToString()    + ", " +
                    yaw.ToString()      + ", " +
                    gyro_x.ToString()   + ", " +
                    gyro_y.ToString()   + ", " +
                    gyro_z.ToString()   + ", " +
                    accel_x.ToString()  + ", " +
                    accel_y.ToString()  + ", " +
                    accel_z.ToString()  + ", " +
                    time.ToString();
        }
    }
}
