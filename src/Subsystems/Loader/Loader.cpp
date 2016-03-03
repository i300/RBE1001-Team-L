#include "loader.hpp"

/*constructor:

 *loaderPin- PWM for a loader pin
 */
Loader::Loader(int8 loaderPin, int8 foamSwitchPin){
  motor = new Motor(loaderPin, false);

  switchPin = foamSwitchPin;
}

/* isFoamLoaded - Checks if a FOAM is loaded
 *
 */
bool16 Loader::isFoamLoaded() {
  // Switch is LOW when closed
  return digitalRead(switchPin) == LOW;
}

/* load- moves a foam through the loader assembly
 *
 */
void Loader::load(){
  motor->write(1);
}

/* unload- removes a foam from the loader assembly
 *
 */
void Loader::unload(){
  motor->write(-1);
}

/* stop- stops the loader motion
 *
 */
void Loader::stop(){
  motor->write(0);
}
