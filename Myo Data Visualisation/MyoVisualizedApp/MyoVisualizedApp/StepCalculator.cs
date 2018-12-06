using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MyoVisualizedApp
{
    class StepCalculator
    {
        private float lastMeasurement;
        private int measureLength;
        private int currentDirection;
        private int[] Direction;
        private int lastDetectedPeek;
        private float directionThreshold;
        private float triggerThreshold;

        public bool step { get; private set; }
        public int DEBUG_arrayOUT { get; private set; }

        public StepCalculator(int measureLength, float measurementThreshold, float triggerThreshold)
        {
            if (measureLength < 0 || measureLength > 20)
            {
                // this is not done 
                throw new IndexOutOfRangeException("madafaka");
            }
            this.measureLength = measureLength;
            this.triggerThreshold = triggerThreshold;
            directionThreshold = measurementThreshold;

            lastMeasurement = 0;
            currentDirection = 0;
            lastDetectedPeek = 0;
            Direction = new int[measureLength];
        }

        public void Calculate(float Sample)
        {
            step = false;
            int average = 0;

            if      (lastMeasurement < Sample - directionThreshold)      currentDirection = -1;
            else if (lastMeasurement > Sample + directionThreshold)      currentDirection = 1;
            else                                                         return;

            lastMeasurement = Sample;

            int[] newArray = new int[Direction.Length];
            Array.Copy(Direction, 1, newArray, 0, Direction.Length - 1);
            Direction = newArray;
            Direction[Direction.Length - 1] = currentDirection;
            lastDetectedPeek++;

            for (int i = 0; i < Direction.Length; i++)
            {
                average += Direction[i];
            }

            DEBUG_arrayOUT = average;
            if (average == 0 && lastDetectedPeek > measureLength && 
               (Sample <= triggerThreshold || Sample >= triggerThreshold))
            {
                lastDetectedPeek = 0;
                step = true;
            }
        }
    }
}
