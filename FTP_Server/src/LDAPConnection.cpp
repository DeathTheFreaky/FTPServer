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

	rc = ldap_simple_bind_s(this->ld, BIND_USER, BIND_PW);

	if(rc != LDAP_SUCCESS){
		std::cerr << "could not login to LDAP-server." << std::endl;
		exit(5);
	}

	std::cout << "bind successful" << std::endl;
}

LDAPConnection::~LDAPConnection(){
	free(attribs[0]);
	ldap_unbind(this->ld);
}

bool LDAPConnection::auth(std::string *username, std::string *pw){
	int rc = 0;
	LDAPMessage *result, *entry;
	std::string filter = "(";
	filter.append(*username);
	filter.append(")");

	rc = ldap_search_s(this->ld, SEARCHBASE, SCOPE, filter.c_str(), this->attribs, 0, &result);
	if(rc != LDAP_SUCCESS){
		return false;
	}

	if(ldap_count_entries(this->ld, result) != 1){
		return false;
	}

	entry = ldap_first_entry(this->ld, result);
	std::string dn = ldap_get_dn(this->ld,entry);

	LDAP *ldp;
	ldap_initialize(&ldp, LDAP_HOST);
	if(ldp == NULL){
		std::cerr << "ldap_initialization failed" << std::endl;
		return false;
	}

	std::cout << "connected to LDAP-server." << std::endl;

	rc = ldap_simple_bind_s(ldp, dn.c_str(), pw->c_str());

	if(rc != LDAP_SUCCESS){
		std::cerr << "could not login to LDAP-server." << std::endl;
		ldap_unbind(ldp);
		return false;
	}

	ldap_unbind(ldp);
	std::cout << "bind successful" << std::endl;
	return true;
}


