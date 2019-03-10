/*
 * network.h
 *	Module to spawn a separate thread to send/ reply a message through UDP.
 *	Host can interact with target using netcat -u 192.168.7.2 8080
 *
 *  Created on: Jun 24, 2018
 *      Author: tudatn
 */

#ifndef NETWORK_H_
#define NETWORK_H_

void Network_startListening(void);

void Network_stopListening(void);

#endif /* NETWORK_H_ */
