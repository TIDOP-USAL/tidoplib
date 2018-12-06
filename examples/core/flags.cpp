#include <stdio.h>

// Cabeceras tidopLib
#include <tidop/core/console.h>
#include <tidop/core/messages.h>
#include <tidop/core/flags.h>

using namespace TL;


enum class ePrueba : int8_t {
  flag_1 = (1 << 0),
  flag_2 = (1 << 1),
  flag_3 = (1 << 2),
  flag_4 = (1 << 3),
  flag_5 = (1 << 4),
  flag_6 = (1 << 5),
  flag_7 = (1 << 6)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba)
 
int main(int argc, char** argv)
{

  /* EnumFlags */

  EnumFlags<ePrueba> flag(ePrueba::flag_1);
 
  // Comprueba si el flag esta activo
  bool bActive = flag.isActive(ePrueba::flag_1);
 
  // Activa un flag
  flag.flagOn(ePrueba::flag_3);
 
  // Desactiva un flag
  flag.flagOff(ePrueba::flag_1);
 
  // Invierte un flag
  flag.switchFlag(ePrueba::flag_5);
 
  /* Operaciones a nivel de bit con un 'enum class' */
  
  // Pueden combinarse los enums
  EnumFlags<ePrueba> flag2;
  flag2 = ePrueba::flag_3 | ePrueba::flag_4;
  
  

  /* Flags */
  Flags_16 flag_16 {1, 5};

  // Comprueba si el flag esta activo
  bActive = flag_16.isActive(1);

  // Activa un flag
  flag_16.flagOn(8);

  // Desactiva un flag
  flag_16.flagOff(1);
 
  // Invierte un flag
  flag_16.switchFlag(5);


  return 0;
}
