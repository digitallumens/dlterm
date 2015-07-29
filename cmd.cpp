#include "cmd.h"

cmd::cmd(QObject *parent, QString helperCmd, QString pmuCmd, parsePmuResponse_t parsePmuResponse) :
  QObject(parent),
  m_helperCmd(helperCmd),
  m_pmuCmd(pmuCmd),
  m_parsePmuResponse(parsePmuResponse) {

}

