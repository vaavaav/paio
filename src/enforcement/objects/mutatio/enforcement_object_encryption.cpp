/**
 *   Written by Pedro Peixoto.
 **/

#include <paio/enforcement/objects/mutatio/enforcement_object_encryption.hpp>
#include <aesxts/xts/xts.hpp>

namespace paio::enforcement {

// EncryptionObject default constructor.
EncryptionObject::EncryptionObject ()
{
    xts_init(64);
    Logging::log_debug ("EncryptionObject default constructor.");
}

// EncryptionObject parameterized constructor.
EncryptionObject::EncryptionObject (const long& object_id) : m_object_id { object_id }
{
    xts_init(64);
    Logging::log_debug ("EncryptionObject parameterized constructor.");
}


// EncryptionObject default destructor.
EncryptionObject::~EncryptionObject ()
{
    std::stringstream stream;
    stream << "EncryptionObject destructor {" << this->m_object_id  << "}";

    Logging::log_debug_explicit (stream.str ());
}

// get_enforcement_object_id call. Get the EnforcementObject's identifier.
long EncryptionObject::get_enforcement_object_id () const
{
    return this->m_object_id;
}

// obj_enforce call. Apply the Noop enforcement mechanisms over I/O workflows.
void EncryptionObject::obj_enforce (const Ticket& ticket, Result& result)
{
    // set the Result's result_status and has_content parameters
    bool has_content = (ticket.get_buffer_size () > 0);
    result.set_result_status (ResultStatus::success);
    result.set_has_content (has_content);

    // if the Ticket contains request's data/metadata, it will be copied to the Result object
    if (has_content) {
        auto tweak = ticket.get_tweak();
        unsigned char tweak_[16] = {0};
        memcpy(tweak_, &tweak, sizeof tweak);
        result.set_content(ticket.get_buffer_size(), ticket.get_buffer());
        result.set_content_size(ticket.get_buffer_size());
        switch (static_cast<paio::core::MUTATIO> (ticket.get_operation_type ())) {
            case MUTATIO::encode: 
                xts_encode (this->key, tweak_, result.get_content(), static_cast<unsigned char*>(ticket.get_buffer()), ticket.get_buffer_size());
                break;
            case MUTATIO::decode:
                xts_decode (this->key, tweak_, result.get_content(), static_cast<unsigned char*>(ticket.get_buffer()), ticket.get_buffer_size());
                break;
            default:
                throw std::runtime_error ("Invalid context type for compression.");
        }
    }
}

// obj_configure call. Configure the internal properties of the Encryption enforcement object.
PStatus EncryptionObject::obj_configure ([[maybe_unused]] int conf,
    [[maybe_unused]] const std::vector<long>& configuration_values)
{
    return PStatus::OK ();
}

// obj_collect_statistics call. Collect I/O statistics from the Encryption enforcement object.
PStatus EncryptionObject::obj_collect_statistics ([[maybe_unused]] ObjectStatisticsRaw& statistics_raw)
{
    return PStatus::Error ();
}

// to_string call. Convert any content of the Noop enforcement object in string-based format.
std::string EncryptionObject::to_string ()
{
    return std::string ("Encryption enforcement object (")
        .append (std::to_string (this->m_object_id))
        .append (").\n");
}

} // namespace paio::enforcement
