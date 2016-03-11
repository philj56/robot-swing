/**

 */

#ifndef EVENTS_EVENTS_H
#define EVENTS_EVENTS_H

#include <boost/shared_ptr.hpp>
#include <alextractor/alextractor.h>
#include <string>
#include <vector>
#include <qi/os.hpp>

#include <alproxies/almemoryproxy.h>

namespace AL
{
  class ALBroker;
}

class GyroEvents : public AL::ALModule
{
  public:

    GyroEvents(boost::shared_ptr<AL::ALBroker> broker, const std::string& name);

    virtual ~GyroEvents();
 
    /**
    * Will be called at module startup. Declares example event and subscribes to
    * it.
    */
    virtual void init();

    /**
    * Callback function every time the example event is called.
    */
    void callback(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg);
    void AverageTurn(const std::string &key, const AL::ALValue &value, const AL::ALValue &msg);
    /**
    * A very simple function to generate the event.
    */
    void generateEvent(const float& value);
    void AnglePeak(const bool& value);
    /**
    * A very simple function to generate the event.
    */
    void generateMicroEvent(const float& value);
    
    void Average();
    void Run();    
    void timer();

  private:
 
    AL::ALMemoryProxy fMemoryProxy;

    AL::ALValue * fGyroY;
    AL::ALValue fCurrentGyro;
    float fgyro;
    
    std::vector<float> lastvalues;
    
    float temp;
    float ftotal;
    float faverage;
    float foldaverage;

    
    float position;
    
    float max, min, amp, omin, omax;
    
    float datapoints;
    
    bool anglepeak, newperiod;
    
    qi::os::timeval startTime;
    qi::os::timeval currentTime;
    qi::os::timeval MovementTime;
    
    float time;
/*
    virtual void xStartDetection(const int pPeriod, const float pPrecision);
    virtual void xStopDetection();
*/
};



#endif  // EVENTS_EVENTS_H

