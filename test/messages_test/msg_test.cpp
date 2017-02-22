#include <gtest/gtest.h>
#include "core\messages.h"
#include "core\utils.h"
#include "experimental\experimental.h"


using namespace I3D;
using namespace EXPERIMENTAL;

TEST(Message, Default) {

  // Valores por defecto
//  //EXPECT_EQ(static_cast<int>(MessageLevel::MSG_ERROR),static_cast<int>(Message::get().getMessageLevel()));
//  //EXPECT_EQ(EnumFlags<MessageOutput>(MessageOutput::MSG_CONSOLE),Message::get().getMessageOutput());

  //Print por defecto
  //Message::message("%s", "Prueba defecto").print();
  //Message::setMessageLevel(MessageLevel::MSG_DEBUG);

//  Message::message("%s", "Prueba Warning").print(MessageLevel::MSG_WARNING, MessageOutput::MSG_CONSOLE | MessageOutput::MSG_LOG);
//  
//  consolePrintDebug("%s", "Prueba consola Debug");
//  consolePrintVerbose("%s", "Prueba consola Verbose");
//  consolePrintInfo("%s", "Prueba consola Info");
//  consolePrintWarning("%s", "Prueba consola Warning");
//  consolePrintError("%s", "Prueba consola Error");
//  
//  logPrintDebug("%s", "Prueba log Debug");
//  logPrintVerbose("%s", "Prueba log Verbose");
//  logPrintInfo("%s", "Prueba log Info");
//  logPrintWarning("%s", "Prueba log Warning");
//  logPrintError("%s", "Prueba log Error");
}

class MsgTest : public MessageManager::Listener
{
public:

  EXPERIMENTAL::MessageLevel mLevel;

  std::string mMsgDebug;
  std::string mMsgVerbose;
  std::string mMsgInfo;
  std::string mMsgWarning;
  std::string mMsgError;

public:
  MsgTest() {}
  ~MsgTest() {}

  void onMsgDebug(const char *msg, char date[64]) override 
  {
    if (mLevel <= EXPERIMENTAL::MessageLevel::MSG_DEBUG) {
      mMsgDebug = msg;
    }
  }

  void onMsgVerbose(const char *msg, char date[64]) override
  {
    if (mLevel <= EXPERIMENTAL::MessageLevel::MSG_VERBOSE) {
      mMsgVerbose = msg;
    }
  }

  void onMsgInfo(const char *msg, char date[64]) override
  {
    if (mLevel <= EXPERIMENTAL::MessageLevel::MSG_INFO) {
      mMsgInfo = msg;
    }
  }

  void onMsgWarning(const char *msg, char date[64]) override
  {
    if (mLevel <= EXPERIMENTAL::MessageLevel::MSG_WARNING) {
      mMsgWarning = msg;
    }
  }

  void onMsgError(const char *msg, char date[64]) override
  {
    if (mLevel <= EXPERIMENTAL::MessageLevel::MSG_ERROR) {
      mMsgError = msg;
    }
  }

};


TEST(MessageManager, Default)
{
  
  MessageManager &msg_h = MessageManager::getInstance();
  MsgTest msgTest;
  // Solo tienen que llegar los mensajes de error
  msgTest.mLevel = EXPERIMENTAL::MessageLevel::MSG_ERROR;
  msg_h.addListener(&msgTest);
  msg_h.release("Prueba Debug 1", EXPERIMENTAL::MessageLevel::MSG_DEBUG);
  EXPECT_STREQ("", msgTest.mMsgDebug.c_str());
  msg_h.release("Prueba Verbose 1", EXPERIMENTAL::MessageLevel::MSG_VERBOSE);
  EXPECT_STREQ("", msgTest.mMsgVerbose.c_str());
  msg_h.release("Prueba Info 1", EXPERIMENTAL::MessageLevel::MSG_INFO);
  EXPECT_STREQ("", msgTest.mMsgInfo.c_str());
  msg_h.release("Prueba Warning 1", EXPERIMENTAL::MessageLevel::MSG_WARNING);
  EXPECT_STREQ("", msgTest.mMsgWarning.c_str());
  msg_h.release("Prueba Error 1", EXPERIMENTAL::MessageLevel::MSG_ERROR);
  EXPECT_STREQ("Prueba Error 1", msgTest.mMsgError.c_str());

  //Prueba con mensajes de warning
  msgTest.mLevel = EXPERIMENTAL::MessageLevel::MSG_WARNING;
  msg_h.release("Prueba Debug 2", EXPERIMENTAL::MessageLevel::MSG_DEBUG);
  EXPECT_STREQ("", msgTest.mMsgDebug.c_str());
  msg_h.release("Prueba Verbose 2", EXPERIMENTAL::MessageLevel::MSG_VERBOSE);
  EXPECT_STREQ("", msgTest.mMsgVerbose.c_str());
  msg_h.release("Prueba Info 2", EXPERIMENTAL::MessageLevel::MSG_INFO);
  EXPECT_STREQ("", msgTest.mMsgInfo.c_str());
  msg_h.release("Prueba Warning 2", EXPERIMENTAL::MessageLevel::MSG_WARNING);
  EXPECT_STREQ("Prueba Warning 2", msgTest.mMsgWarning.c_str());
  msg_h.release("Prueba Error 2", EXPERIMENTAL::MessageLevel::MSG_ERROR);
  EXPECT_STREQ("Prueba Error 2", msgTest.mMsgError.c_str());

  //Prueba con mensajes de info
  msgTest.mLevel = EXPERIMENTAL::MessageLevel::MSG_INFO;
  msg_h.release("Prueba Debug 3", EXPERIMENTAL::MessageLevel::MSG_DEBUG);
  EXPECT_STREQ("", msgTest.mMsgDebug.c_str());
  msg_h.release("Prueba Verbose 3", EXPERIMENTAL::MessageLevel::MSG_VERBOSE);
  EXPECT_STREQ("", msgTest.mMsgVerbose.c_str());
  msg_h.release("Prueba Info 3", EXPERIMENTAL::MessageLevel::MSG_INFO);
  EXPECT_STREQ("Prueba Info 3", msgTest.mMsgInfo.c_str());
  msg_h.release("Prueba Warning 3", EXPERIMENTAL::MessageLevel::MSG_WARNING);
  EXPECT_STREQ("Prueba Warning 3", msgTest.mMsgWarning.c_str());
  msg_h.release("Prueba Error 3", EXPERIMENTAL::MessageLevel::MSG_ERROR);
  EXPECT_STREQ("Prueba Error 3", msgTest.mMsgError.c_str());

  //Prueba con mensajes de verbose
  msgTest.mLevel = EXPERIMENTAL::MessageLevel::MSG_VERBOSE;
  msg_h.release("Prueba Debug 4", EXPERIMENTAL::MessageLevel::MSG_DEBUG);
  EXPECT_STREQ("", msgTest.mMsgDebug.c_str());
  msg_h.release("Prueba Verbose 4", EXPERIMENTAL::MessageLevel::MSG_VERBOSE);
  EXPECT_STREQ("Prueba Verbose 4", msgTest.mMsgVerbose.c_str());
  msg_h.release("Prueba Info 4", EXPERIMENTAL::MessageLevel::MSG_INFO);
  EXPECT_STREQ("Prueba Info 4", msgTest.mMsgInfo.c_str());
  msg_h.release("Prueba Warning 4", EXPERIMENTAL::MessageLevel::MSG_WARNING);
  EXPECT_STREQ("Prueba Warning 4", msgTest.mMsgWarning.c_str());
  msg_h.release("Prueba Error 4", EXPERIMENTAL::MessageLevel::MSG_ERROR);
  EXPECT_STREQ("Prueba Error 4", msgTest.mMsgError.c_str());


  //Prueba con mensajes debug
  msgTest.mLevel = EXPERIMENTAL::MessageLevel::MSG_DEBUG;
  msg_h.release("Prueba Debug 5", EXPERIMENTAL::MessageLevel::MSG_DEBUG);
  EXPECT_STREQ("Prueba Debug 5", msgTest.mMsgDebug.c_str());
  msg_h.release("Prueba Verbose 5", EXPERIMENTAL::MessageLevel::MSG_VERBOSE);
  EXPECT_STREQ("Prueba Verbose 5", msgTest.mMsgVerbose.c_str());
  msg_h.release("Prueba Info 5", EXPERIMENTAL::MessageLevel::MSG_INFO);
  EXPECT_STREQ("Prueba Info 5", msgTest.mMsgInfo.c_str());
  msg_h.release("Prueba Warning 5", EXPERIMENTAL::MessageLevel::MSG_WARNING);
  EXPECT_STREQ("Prueba Warning 5", msgTest.mMsgWarning.c_str());
  msg_h.release("Prueba Error 5", EXPERIMENTAL::MessageLevel::MSG_ERROR);
  EXPECT_STREQ("Prueba Error 5", msgTest.mMsgError.c_str());
}

// Prueba de la clase interna Message
TEST(MessageManager, Message)
{
  EXPECT_STREQ("mensaje 1", MessageManager::Message("mensaje %i", 1).getMessage());
}

GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
