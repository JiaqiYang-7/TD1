#include "mbed.h"


class Potentiometer                                 //Begin Potentiometer class definition
//base class for smapling potentionmeter
    {
    private:                                            //Private data member declaration; will be private in derived class aslo
    AnalogIn inputSignal;                           //Declaration of AnalogIn object
    float VDD, currentSampleNorm, currentSampleVolts; //Float variables to speficy the value of VDD and most recent samples

    public:                                             // Public declarations
    Potentiometer(PinName pin, float v) : inputSignal(pin), VDD(v) {}   //Constructor - user provided pin name assigned to AnalogIn...
                                                                        //VDD is also provided to determine maximum measurable voltage
    float amplitudeVolts(void)                      //Public member function to measure the amplitude in volts
    {
        return (inputSignal.read()*VDD);            //Scales the 0.0-1.0 value by VDD to read the input in volts
    }

    float amplitudeNorm(void)                       //Public member function to measure the normalised amplitude
    {
        return inputSignal.read();                  //Returns the ADC value normalised to range 0.0 - 1.0
    }

    void sample(void)                               //Public member function to sample an analogue voltage
    {
        currentSampleNorm = inputSignal.read();       //Stores the current ADC value to the class's data member for normalised values (0.0 - 1.0)
        currentSampleVolts = currentSampleNorm * VDD; //Converts the normalised value to the equivalent voltage (0.0 - 3.3 V) and stores this information
    }

    float getCurrentSampleVolts(void)               //Public member function to return the most recent sample from the potentiometer (in volts)
    {
        return currentSampleVolts;                  //Return the contents of the data member currentSampleVolts
    }

    float getCurrentSampleNorm(void)                //Public member function to return the most recent sample from the potentiometer (normalised)
    {
        return currentSampleNorm;                   //Return the contents of the data member currentSampleNorm  
    }
    };

class SamplingPotentiometer : public Potentiometer { //create a derived class based o the base class of potentiometer, allows us to use all the data func declared in potentiometer class
    //samplingpoentiometer inherits class potentiometer
    //inhertitance: defines one class in terms of another class; adds more member functions and data memebers to the base class
    private: //declare private data member
    float samplingFrequency, samplingPeriod;
    Ticker sampler; //data member sampler of type ticker
    public:
    SamplingPotentiometer(PinName p, float v, float fs): Potentiometer(p, v),samplingFrequency(fs){ //constructor
    samplingPeriod = 1.0f / samplingFrequency; //calculate period from frequency
    sampler.attach(callback(this, &Potentiometer::sample), samplingPeriod); // attach callback to member function sampler,
    //"this" referes to the current specific SamplingPotentiometer object being created
    //'this' pointer used to specify the object's data member
    };
    };