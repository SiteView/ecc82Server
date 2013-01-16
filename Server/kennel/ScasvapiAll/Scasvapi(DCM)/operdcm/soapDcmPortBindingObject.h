/* soapDcmPortBindingObject.h
   Generated by gSOAP 2.7.12 from dcm.h
   Copyright(C) 2000-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapDcmPortBindingObject_H
#define soapDcmPortBindingObject_H
#include "soapH.h"

/******************************************************************************\
 *                                                                            *
 * Service Object                                                             *
 *                                                                            *
\******************************************************************************/

class DcmPortBindingService : public soap
{  
public:
	DcmPortBindingService()
	{ static const struct Namespace namespaces[] =
	{
		{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
		{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
		{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
		{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
		{"ns1", "http://wsdl.intf.dcm.intel.com/", NULL, NULL},
		{NULL, NULL, NULL, NULL}
	};
	if (!this->namespaces) this->namespaces = namespaces; };
	virtual ~DcmPortBindingService() { };
	/// Bind service to port (returns master socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET bind(const char *host, int port, int backlog) { return soap_bind(this, host, port, backlog); };
	/// Accept next request (returns socket or SOAP_INVALID_SOCKET)
	virtual	SOAP_SOCKET accept() { return soap_accept(this); };
	/// Serve this request (returns error code or SOAP_OK)
	virtual	int serve() { return soap_serve(this); };
};

/******************************************************************************\
 *                                                                            *
 * Service Operations (you should define these globally)                      *
 *                                                                            *
\******************************************************************************/


SOAP_FMAC5 int SOAP_FMAC6 __ns1__getVersion(struct soap*, ns1__getVersion *ns1__getVersion_, ns1__getVersionResponse *ns1__getVersionResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__setPolicy(struct soap*, ns1__setPolicy *ns1__setPolicy_, ns1__setPolicyResponse *ns1__setPolicyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__updatePolicy(struct soap*, ns1__updatePolicy *ns1__updatePolicy_, ns1__updatePolicyResponse *ns1__updatePolicyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__addEntity(struct soap*, ns1__addEntity *ns1__addEntity_, ns1__addEntityResponse *ns1__addEntityResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__associateEntity(struct soap*, ns1__associateEntity *ns1__associateEntity_, ns1__associateEntityResponse *ns1__associateEntityResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__defineCustomEvent(struct soap*, ns1__defineCustomEvent *ns1__defineCustomEvent_, ns1__defineCustomEventResponse *ns1__defineCustomEventResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__deleteSecretKey(struct soap*, ns1__deleteSecretKey *ns1__deleteSecretKey_, ns1__deleteSecretKeyResponse *ns1__deleteSecretKeyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__disassociateEntity(struct soap*, ns1__disassociateEntity *ns1__disassociateEntity_, ns1__disassociateEntityResponse *ns1__disassociateEntityResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__enumerateCustomEvents(struct soap*, ns1__enumerateCustomEvents *ns1__enumerateCustomEvents_, ns1__enumerateCustomEventsResponse *ns1__enumerateCustomEventsResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__enumerateEntities(struct soap*, ns1__enumerateEntities *ns1__enumerateEntities_, ns1__enumerateEntitiesResponse *ns1__enumerateEntitiesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__enumerateEventHandlers(struct soap*, ns1__enumerateEventHandlers *ns1__enumerateEventHandlers_, ns1__enumerateEventHandlersResponse *ns1__enumerateEventHandlersResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__enumeratePolicies(struct soap*, ns1__enumeratePolicies *ns1__enumeratePolicies_, ns1__enumeratePoliciesResponse *ns1__enumeratePoliciesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__exportHierarchy(struct soap*, ns1__exportHierarchy *ns1__exportHierarchy_, ns1__exportHierarchyResponse *ns1__exportHierarchyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__findEntities(struct soap*, ns1__findEntities *ns1__findEntities_, ns1__findEntitiesResponse *ns1__findEntitiesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getActionLogRecords(struct soap*, ns1__getActionLogRecords *ns1__getActionLogRecords_, ns1__getActionLogRecordsResponse *ns1__getActionLogRecordsResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getCollectionState(struct soap*, ns1__getCollectionState *ns1__getCollectionState_, ns1__getCollectionStateResponse *ns1__getCollectionStateResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getCustomEventData(struct soap*, ns1__getCustomEventData *ns1__getCustomEventData_, ns1__getCustomEventDataResponse *ns1__getCustomEventDataResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getEntityProperties(struct soap*, ns1__getEntityProperties *ns1__getEntityProperties_, ns1__getEntityPropertiesResponse *ns1__getEntityPropertiesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getEventHandler(struct soap*, ns1__getEventHandler *ns1__getEventHandler_, ns1__getEventHandlerResponse *ns1__getEventHandlerResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getEventState(struct soap*, ns1__getEventState *ns1__getEventState_, ns1__getEventStateResponse *ns1__getEventStateResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getGlobalProperty(struct soap*, ns1__getGlobalProperty *ns1__getGlobalProperty_, ns1__getGlobalPropertyResponse *ns1__getGlobalPropertyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getMetricData(struct soap*, ns1__getMetricData *ns1__getMetricData_, ns1__getMetricDataResponse *ns1__getMetricDataResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getNextData(struct soap*, ns1__getNextData *ns1__getNextData_, ns1__getNextDataResponse *ns1__getNextDataResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getPolicyData(struct soap*, ns1__getPolicyData *ns1__getPolicyData_, ns1__getPolicyDataResponse *ns1__getPolicyDataResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getPolicyHistory(struct soap*, ns1__getPolicyHistory *ns1__getPolicyHistory_, ns1__getPolicyHistoryResponse *ns1__getPolicyHistoryResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getPolicyState(struct soap*, ns1__getPolicyState *ns1__getPolicyState_, ns1__getPolicyStateResponse *ns1__getPolicyStateResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getPredefinedEventTypes(struct soap*, ns1__getPredefinedEventTypes *ns1__getPredefinedEventTypes_, ns1__getPredefinedEventTypesResponse *ns1__getPredefinedEventTypesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__getQueryData(struct soap*, ns1__getQueryData *ns1__getQueryData_, ns1__getQueryDataResponse *ns1__getQueryDataResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__importHierarchy(struct soap*, ns1__importHierarchy *ns1__importHierarchy_, ns1__importHierarchyResponse *ns1__importHierarchyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__isPolicyActive(struct soap*, ns1__isPolicyActive *ns1__isPolicyActive_, ns1__isPolicyActiveResponse *ns1__isPolicyActiveResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__rediscoverNodeCapabilities(struct soap*, ns1__rediscoverNodeCapabilities *ns1__rediscoverNodeCapabilities_, ns1__rediscoverNodeCapabilitiesResponse *ns1__rediscoverNodeCapabilitiesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__removeCustomEvent(struct soap*, ns1__removeCustomEvent *ns1__removeCustomEvent_, ns1__removeCustomEventResponse *ns1__removeCustomEventResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__removeEntity(struct soap*, ns1__removeEntity *ns1__removeEntity_, ns1__removeEntityResponse *ns1__removeEntityResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__removePolicy(struct soap*, ns1__removePolicy *ns1__removePolicy_, ns1__removePolicyResponse *ns1__removePolicyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__schedulePolicy(struct soap*, ns1__schedulePolicy *ns1__schedulePolicy_, ns1__schedulePolicyResponse *ns1__schedulePolicyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__setCollectionState(struct soap*, ns1__setCollectionState *ns1__setCollectionState_, ns1__setCollectionStateResponse *ns1__setCollectionStateResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__setEntityProperties(struct soap*, ns1__setEntityProperties *ns1__setEntityProperties_, ns1__setEntityPropertiesResponse *ns1__setEntityPropertiesResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__setEventState(struct soap*, ns1__setEventState *ns1__setEventState_, ns1__setEventStateResponse *ns1__setEventStateResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__setGlobalProperty(struct soap*, ns1__setGlobalProperty *ns1__setGlobalProperty_, ns1__setGlobalPropertyResponse *ns1__setGlobalPropertyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__setPolicyState(struct soap*, ns1__setPolicyState *ns1__setPolicyState_, ns1__setPolicyStateResponse *ns1__setPolicyStateResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__submitSecretKey(struct soap*, ns1__submitSecretKey *ns1__submitSecretKey_, ns1__submitSecretKeyResponse *ns1__submitSecretKeyResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__subscribeEventHandler(struct soap*, ns1__subscribeEventHandler *ns1__subscribeEventHandler_, ns1__subscribeEventHandlerResponse *ns1__subscribeEventHandlerResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__subscribePredefinedEvent(struct soap*, ns1__subscribePredefinedEvent *ns1__subscribePredefinedEvent_, ns1__subscribePredefinedEventResponse *ns1__subscribePredefinedEventResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__unsubscribeEventHandler(struct soap*, ns1__unsubscribeEventHandler *ns1__unsubscribeEventHandler_, ns1__unsubscribeEventHandlerResponse *ns1__unsubscribeEventHandlerResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__unsubscribePredefinedEvent(struct soap*, ns1__unsubscribePredefinedEvent *ns1__unsubscribePredefinedEvent_, ns1__unsubscribePredefinedEventResponse *ns1__unsubscribePredefinedEventResponse_);

SOAP_FMAC5 int SOAP_FMAC6 __ns1__updateCustomEvent(struct soap*, ns1__updateCustomEvent *ns1__updateCustomEvent_, ns1__updateCustomEventResponse *ns1__updateCustomEventResponse_);

#endif
