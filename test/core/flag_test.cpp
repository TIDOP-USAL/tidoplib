#include <gtest/gtest.h>

#include "core/flags.h"

#include <cstdint>

using namespace TL;

//enums para pruebas

// unscoped enumeration type
enum class ePrueba1 {

};

enum class ePrueba2 : int8_t {
  flag00 = (0 << 0),
  flag01 = (1 << 0),
  flag02 = (1 << 1),
  flag03 = (1 << 2),
  flag04 = (1 << 3),
  flag05 = (1 << 4),
  flag06 = (1 << 5),
  flag07 = (1 << 6)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba2)

enum class ePrueba3 : int16_t {
  flag00 = (0 << 0),
  flag01 = (1 << 0),
  flag02 = (1 << 1),
  flag03 = (1 << 2),
  flag04 = (1 << 3),
  flag05 = (1 << 4),
  flag06 = (1 << 5),
  flag07 = (1 << 6),
  flag08 = (1 << 7),
  flag09 = (1 << 8),
  flag10 = (1 << 9),
  flag11 = (1 << 10),
  flag12 = (1 << 11),
  flag13 = (1 << 12),
  flag14 = (1 << 13),
  flag15 = (1 << 14)
};

ALLOW_BITWISE_FLAG_OPERATIONS(ePrueba3)


// Constructor EnumFlags
TEST(EnumFlags, Constructor) 
{

  // Constructor vacio
  EnumFlags<ePrueba2> flagEmpty;
  EXPECT_EQ(0, static_cast<EnumFlags<ePrueba2>::Type>(flagEmpty.getFlags()));

  // Constructor copia
  EnumFlags<ePrueba2> flag_copy(flagEmpty);
  EXPECT_EQ(0, static_cast<EnumFlags<ePrueba2>::Type>(flag_copy.getFlags()));

  // Constructor por enun
  EnumFlags<ePrueba2> flag(ePrueba2::flag01);

  EXPECT_TRUE(flag.isActive(ePrueba2::flag01));
  EXPECT_FALSE(flag.isActive(ePrueba2::flag04));
}

TEST(EnumFlags, Assign) 
{

  EnumFlags<ePrueba2> flagEmpty;
  flagEmpty = ePrueba2::flag01;

  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag01));

  flagEmpty = ePrueba2::flag02 | ePrueba2::flag03;

  EXPECT_FALSE(flagEmpty.isActive(ePrueba2::flag01));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag02));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag03));

  flagEmpty = ePrueba2::flag02 | ePrueba2::flag03 | ePrueba2::flag01;
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag01));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag02));
  EXPECT_TRUE(flagEmpty.isActive(ePrueba2::flag03));
}

TEST(EnumFlags, Clear)
{

  EnumFlags<ePrueba2> flag(ePrueba2::flag02 | ePrueba2::flag03);
  EXPECT_TRUE(flag.isActive(ePrueba2::flag02));
  EXPECT_TRUE(flag.isActive(ePrueba2::flag03));
  flag.clear();
  EXPECT_FALSE(flag.isActive(ePrueba2::flag02));
  EXPECT_FALSE(flag.isActive(ePrueba2::flag03));

}

TEST(EnumFlags, flagOnOff) 
{

  EnumFlags<ePrueba3> flag2(ePrueba3::flag15);

  EXPECT_TRUE(flag2.isActive(ePrueba3::flag15));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag03));

  flag2.flagOff(ePrueba3::flag15);
  flag2.flagOn(ePrueba3::flag03);

  EXPECT_FALSE(flag2.isActive(ePrueba3::flag15));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag03));

  //Prueba a activar un flag ya activo
  ePrueba3 ef = flag2.getFlags();
  flag2.flagOn(ePrueba3::flag03);
  ePrueba3 ef2 = flag2.getFlags();
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag03));
}

TEST(EnumFlags, switchFlag)
{

  EnumFlags<ePrueba3> flag2(ePrueba3::flag15 | ePrueba3::flag06 );
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag15));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag06));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag01));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag03));

  flag2.switchFlag(ePrueba3::flag15);
  flag2.switchFlag(ePrueba3::flag06);
  flag2.switchFlag(ePrueba3::flag01);
  flag2.switchFlag(ePrueba3::flag03);

  EXPECT_FALSE(flag2.isActive(ePrueba3::flag15));
  EXPECT_FALSE(flag2.isActive(ePrueba3::flag06));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag01));
  EXPECT_TRUE(flag2.isActive(ePrueba3::flag03));

}



// Constructor Flags
TEST(Flags, Constructor)
{

  // Constructor vacio
  Flags_8 flag8;
  EXPECT_EQ(0, static_cast<EnumFlags<ePrueba2>::Type>(flag8.getFlags()));
  EXPECT_FALSE(flag8.isActive(3));

  Flags_16 flag16;
  EXPECT_EQ(0, static_cast<EnumFlags<ePrueba2>::Type>(flag16.getFlags()));
  EXPECT_FALSE(flag16.isActive(10));

  Flags_32 flag32;
  EXPECT_EQ(0, static_cast<EnumFlags<ePrueba2>::Type>(flag32.getFlags()));
  EXPECT_FALSE(flag32.isActive(25));

  Flags_64 flag64;
  EXPECT_EQ(0, static_cast<EnumFlags<ePrueba2>::Type>(flag64.getFlags()));
  EXPECT_FALSE(flag64.isActive(45));

  // Constructor copia
  flag16.flagOn(3);
  Flags<uint16_t> flag_copy(flag16);
  EXPECT_TRUE(flag_copy.isActive(3));
  EXPECT_FALSE(flag_copy.isActive(4));

  // Construcción con inicialización de lista
  Flags_32 flag_list{ 0, 3, 7, 4 };
  EXPECT_TRUE(flag_list.isActive(0));
  EXPECT_TRUE(flag_list.isActive(3));
  EXPECT_TRUE(flag_list.isActive(7));
  EXPECT_TRUE(flag_list.isActive(4));
}

TEST(Flags, Assign)
{

  Flags_8 flag1;
  flag1.flagOn(1);

  Flags_8 flag2 = flag1;

  EXPECT_TRUE(flag2.isActive(1));
  EXPECT_FALSE(flag2.isActive(0));
  EXPECT_FALSE(flag2.isActive(2));
}

TEST(Flags, Clear)
{

  Flags_32 flag;
  flag.flagOn(2);
  flag.switchFlag(3);
  EXPECT_TRUE(flag.isActive(2));
  EXPECT_TRUE(flag.isActive(3));
  flag.clear();
  EXPECT_FALSE(flag.isActive(2));
  EXPECT_FALSE(flag.isActive(3));

}

TEST(Flags, flagOnOff)
{

  Flags_64 flag2;
  flag2.flagOn(15);

  EXPECT_TRUE(flag2.isActive(15));
  EXPECT_FALSE(flag2.isActive(3));

  flag2.flagOff(15);
  flag2.flagOn(3);

  EXPECT_FALSE(flag2.isActive(15));
  EXPECT_TRUE(flag2.isActive(3));

  //Prueba a activar un flag ya activo
  flag2.flagOn(3);
  EXPECT_TRUE(flag2.isActive(3));
}

TEST(Flags, switchFlag) 
{

  Flags_16 flag_16;
  flag_16.switchFlag(15);
  flag_16.switchFlag(6);
  EXPECT_TRUE(flag_16.isActive(15));
  EXPECT_TRUE(flag_16.isActive(6));
  EXPECT_FALSE(flag_16.isActive(1));
  EXPECT_FALSE(flag_16.isActive(3));

  flag_16.switchFlag(15);
  flag_16.switchFlag(6);
  flag_16.switchFlag(1);
  flag_16.switchFlag(3);

  EXPECT_FALSE(flag_16.isActive(15));
  EXPECT_FALSE(flag_16.isActive(6));
  EXPECT_TRUE(flag_16.isActive(1));
  EXPECT_TRUE(flag_16.isActive(3));

}

TEST(Flags, bad_data)
{
  Flags_16 flag_16;
  flag_16.flagOn(5);
  flag_16.flagOn(25); // Se asigna un valor fuera de rango
  EXPECT_FALSE(flag_16.isActive(25));
  EXPECT_TRUE(flag_16.isActive(5));
}
