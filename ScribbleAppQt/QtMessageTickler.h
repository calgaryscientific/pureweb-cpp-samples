#ifndef QTMESSAGETICKLER_H
#define QTMESSAGETICKLER_H

class QtMessageTickler : public QObject, public CSI::PureWeb::Server::IResponseProvider, public CSI::PureWeb::Server::IStateManagerPlugin
{
   private:
      CSI::Collections::List<CSI::PureWeb::Server::ResponseInfo> m_empty;
      CSI::PureWeb::Server::StateManager * m_pStateManager;
      int m_eventCounter;
   public:
      QtMessageTickler()
      {
         m_eventCounter = 0;
      }

      bool event(QEvent * event)
      {
         if (event->type() == QEvent::User)
         {
            CSI::Threading::Interlocked::Exchange(&m_eventCounter, 0);
            CSI::Threading::UiDispatcher::PumpMessages();
            return true;
         }

         return false;
      }

      void SessionConnected(CSI::Guid sessionId, CSI::Typeless const& command)
      {
         m_pStateManager->SessionManager().AddResponseProvider(sessionId, this);

      }

      void SessionDisconnected(CSI::Guid sessionId, CSI::Typeless const& command)
      {
         m_pStateManager->SessionManager().RemoveResponseProvider(sessionId, this);
      }

      CSI::Collections::List<CSI::PureWeb::Server::ResponseInfo> GetNextResponses(CSI::Guid sessionId)
      {
         if (CSI::Threading::Interlocked::CompareExchange(&m_eventCounter, 1, 0))
         {
            QEvent * event = new QEvent(QEvent::User);
            QApplication::postEvent(this,event);
         }

         return m_empty;
      }

      void Initialize(CSI::PureWeb::Server::StateManager * pStateManager)
      {

         if (pStateManager == NULL)
            CSI_THROW(CSI::ArgumentNullException, "pStateManager");
         m_pStateManager = pStateManager;
      }

      void Uninitialize() {}

};

#endif // QTMESSAGETICKLER_H
