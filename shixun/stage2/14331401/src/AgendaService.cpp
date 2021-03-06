#include "AgendaService.h"

AgendaService::AgendaService() {
  startAgenda();
}

AgendaService::~AgendaService() {}

bool AgendaService::userRegister(std::string userName, std::string password,
                              std::string email, std::string phone) {

  std::list<User> li;
  li = storage_->queryUser([&](const User& user)->bool
                           {return user.getName() == userName;});
  if (!li.empty()) {
    return false;
  } else {
    User user(userName, password, email, phone);
    storage_->createUser(user);
    return true;
  }
}

bool AgendaService::userLogIn(std::string userName, std::string password) {
  std::list<User> li;
  li = storage_->queryUser([&](const User& user)->bool
                           {return user.getName() == userName;});
  if (li.empty()) {
    return false;
  } else {
    std::list<User>::iterator it = li.begin();

    if (it->getPassword() == password) {
      return true;
    } else {
      return false;
    }

  }
}

bool AgendaService::deleteUser(std::string userName, std::string password) {
  if (storage_->deleteUser([&](const User& user)->bool
                           {return (user.getName() == userName &&
                                    user.getPassword() == password);})) {
    //  if this user exists, delete meetings which related to him
    storage_->deleteMeeting([&](const Meeting& m)
                            {return (m.getSponsor() == userName ||
                                     m.getParticipator() == userName);});
    return true;
  }
  return false;
}

std::list<User> AgendaService::listAllUsers() {
  return storage_->queryUser([](const User& user)->bool {return true;});
}

bool AgendaService::createMeeting(std::string userName, std::string title,
                                  std::string participator,
                                  std::string startDate, std::string endDate) {
  Date sta = Date::stringToDate(startDate),
    ed = Date::stringToDate(endDate);
  //  check if the time is valid
  if (sta >= ed || !Date::isValid(sta) || !Date::isValid(ed)) return false;

  //  check if they are all in the list
  if (storage_->queryUser([&](const User& user)
                            {return user.getName() == userName;}).empty() ||
      storage_->queryUser([&](const User& user)
                           {return user.getName() == participator;}).empty())
    return false;

  //  check if they are free
  if (meetingQuery(userName, startDate, endDate).empty() &&
      meetingQuery(participator, startDate, endDate).empty()) {
    Meeting m(userName, participator, sta, ed, title);
    storage_->createMeeting(m);
    return true;
  }
return false;
}

std::list<Meeting> AgendaService::meetingQuery(std::string userName,
                                               std::string title) {
  std::list<Meeting> li;
  li = storage_->queryMeeting([&](const Meeting& m)->bool
                              {
                                if (m.getTitle() == title &&
                                   (m.getSponsor() == userName ||
                                    m.getParticipator() == userName))
                                  return true;
                                return false;
                              });
  return li;
}

std::list<Meeting>
AgendaService::meetingQuery(std::string userName,
                            std::string startDate, std::string endDate) {
  std::list<Meeting> li;
  Date sta = Date::stringToDate(startDate),
    ed = Date::stringToDate(endDate);
  //  check if the input is valid
  if (sta < ed && Date::isValid(sta) && Date::isValid(ed)) {
      li = storage_->queryMeeting([&](const Meeting& m)->bool
                              {
                                if ((m.getSponsor() == userName ||
                                     m.getParticipator() == userName) &&
                                    !(m.getStartDate() > ed &&
                                      m.getEndDate() < sta))
                                    return true;
                                return false;
                              });
  }
  return li;
}

std::list<Meeting> AgendaService::listAllMeetings(std::string userName) {
  std::list<Meeting> li;
  li = storage_->queryMeeting([&](const Meeting& m)->bool
                              {
                                if (m.getSponsor() == userName ||
                                    m.getParticipator() == userName)
                                  return true;
                                return false;
                              });
  return li;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(std::string userName) {
  std::list<Meeting> li;
  li = storage_->queryMeeting([&](const Meeting& m)->bool
                              {
                                if (m.getSponsor() == userName)
                                  return true;
                                return false;
                              });
  return li;
}

std::list<Meeting>
AgendaService::listAllParticipateMeetings(std::string userName) {
  std::list<Meeting> li;
  li = storage_->queryMeeting([&](const Meeting& m)->bool
                              {
                                if (m.getParticipator() == userName)
                                  return true;
                                return false;
                              });
  return li;
}

bool AgendaService::deleteMeeting(std::string userName, std::string title) {
  int count = storage_->deleteMeeting([&](const Meeting& m)
                                      {
                                        if (m.getTitle() == title &&
                                            (m.getSponsor() == userName ||
                                             m.getParticipator() == userName))
                                          return true;
                                        return false;
                                      });
  if (count)
    return true;
  else
    return false;
}

bool AgendaService::deleteAllMeetings(std::string userName) {
  int count = storage_->deleteMeeting([&](const Meeting& m)
                                      {
                                        if (m.getSponsor() == userName ||
                                             m.getParticipator() == userName)
                                          return true;
                                        return false;
                                      });
  if (count)
    return true;
  else
    return false;
}

void AgendaService::startAgenda() {
  storage_ = Storage::getInstance();
}

void AgendaService::quitAgenda() {}
