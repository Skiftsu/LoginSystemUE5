#include "Utils/Descriptors.h"
#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"

const google::protobuf::FieldDescriptor* Descriptors::loginStatus_ich =
	CheckRegParameterStatus::default_instance().descriptor()->FindFieldByName("loginStatus");
const google::protobuf::FieldDescriptor* Descriptors::mailStatus_ich =
	CheckRegParameterStatus::default_instance().descriptor()->FindFieldByName("mailStatus");