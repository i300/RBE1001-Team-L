#include "loader.hpp"

/*constructor:

 *loaderPin- PWM for a loader pin
 */
Loader::Loader(int loaderPin){
  motor = new Motor(loaderPin, false);
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
