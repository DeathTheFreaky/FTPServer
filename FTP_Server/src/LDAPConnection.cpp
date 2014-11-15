/*
 * LDAP.cpp
 *
 *  Created on: 15.11.2014
 *      Author: clemens
 */

#include "LDAPConnection.h"

LDAPConnection::LDAPConnection(){
	int rc = 0;
	attribs[0] = strdup("uid");
	attribs[1] = NULL;

	ldap_initialize(&this->ld, LDAP_HOST);
	if(this->ld == NULL){
		std::cerr << "ldap_initialization failed" << std::endl;
		exit(4);
	}

	std::cout << "connected to LDAP-server." << std::endl;

	rc = ldap_sasl_bind

	if(rc != LDAP_SUCCESS){
		std::cerr << "could not login to LDAP-server." << std::endl;
		exit(5);
	}

}

LDAPConnection::~LDAPConnection(){

}
