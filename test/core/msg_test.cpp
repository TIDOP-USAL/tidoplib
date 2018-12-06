#include "config_tl.h"

#include <gtest/gtest.h>
#ifdef HAVE_GMOCK
#include <gmock/gmock.h>
#endif
#include <tidop/core/messages.h>

using namespace TL;

#ifdef HAVE_GMOCK
using ::testing::_;

class MockMessageManagerListener : public MessageManager::Listener
{
public:

  MockMessageManagerListener(bool add = true)
    : MessageManager::Listener(add){}

  MOCK_METHOD2(onMsgDebug, void(const char *msg, const char *date));
  MOCK_METHOD2(onMsgInfo, void(const char *msg, const char *date));
  MOCK_METHOD2(onMsgWarning, void(const char *msg, const char *date));
  MOCK_METHOD2(onMsgError, void(const char *msg, const char *date));
};


TEST(MessageManager, addListener)
{

//  MessageManager &msg_h = MessageManager::getInstance();
//  MockMessageManagerListener listenerImpl;

//  //msg_h.addListener(&listenerImpl);

//  msgInfo("Mensaje de ejemplo");

//  //Problema con el segundo parametro ya que se rellena desde MessageManager y no lo conozco...
//  EXPECT_CALL(listenerImpl, onMsgInfo(_, _))
//          .Times(1);


}

#endif