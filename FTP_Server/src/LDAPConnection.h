/*
 * LDAP.h
 *
 *  Created on: 15.11.2014
 *      Author: clemens
 */

#ifndef LDAPCONNECTION_H_
#define LDAPCONNECTION_H_

#include <string>
#include <stdlib.h>
#include <iostream>
#include <ldap.h>

#define LDAP_HOST "ldap://ldap.technikum-wien.at:389"
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define BIND_USER "uid=if13b056,ou=People,dc=technikum-wien,dc=at"
#define BIND_PW "13OPy17AM"

class LDAPConnection {
private:
	LDAP *ld;
	LDAPMessage *result, *e;
	char *attribs[2];
protected:

public:
	LDAPConnection();
	virtual ~LDAPConnection();

	bool auth(std::string *username, std::string *pw);
};

#endif /* LDAPCONNECTION_H_ */
