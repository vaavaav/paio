/**
 *   Written by Pedro Peixoto.
 **/

#include <paio/enforcement/objects/mutatio/enforcement_object_compression.hpp>
#include <snappy.h>

namespace paio::enforcement {

// CompressionObject default constructor.
CompressionObject::CompressionObject ()
{
    Logging::log_debug ("CompressionObject default constructor.");
}

// CompressionObject parameterized constructor.
CompressionObject::CompressionObject (const long& object_id) : m_object_id { object_id }
{
    Logging::log_debug ("CompressionObject parameterized constructor.");
}


// CompressionObject default destructor.
CompressionObject::~CompressionObject ()
{
    std::stringstream stream;
    stream << "CompressionObject destructor {" << this->m_object_id  << "}";

    Logging::log_debug_explicit (stream.str ());
}

// get_enforcement_object_id call. Get the EnforcementObject's identifier.
long CompressionObject::get_enforcement_object_id () const
{
    return this->m_object_id;
}

// obj_enforce call. Apply the Noop enforcement mechanisms over I/O workflows.
void CompressionObject::obj_enforce (const Ticket& ticket, Result& result)
{
    // set the Result's result_status and has_content parameters
    bool has_content = (ticket.get_buffer_size () > 0);
    result.set_result_status (ResultStatus::success);
    result.set_has_content (has_content);

    // if the Ticket contains request's data/metadata, it will be copied to the Result object
    if (has_content) {
        std::string s; 
        switch (static_cast<paio::core::MUTATIO>(ticket.get_operation_type())) {
            case MUTATIO::encode:
                snappy::Compress((const char*) ticket.get_buffer(), ticket.get_buffer_size(), &s);
                break;
            case MUTATIO::decode:
                snappy::Uncompress((const char*) ticket.get_buffer(), ticket.get_buffer_size(), &s);
                break;
        }
        result.set_content(s.length(), (const unsigned char*) s.data());
        result.set_content_size(s.length());
    }
}

// obj_configure call. Configure the internal properties of the Compression enforcement object.
PStatus CompressionObject::obj_configure ([[maybe_unused]] int conf,
    [[maybe_unused]] const std::vector<long>& configuration_values)
{
    return PStatus::OK ();
}

// obj_collect_statistics call. Collect I/O statistics from the Compression enforcement object.
PStatus CompressionObject::obj_collect_statistics ([[maybe_unused]] ObjectStatisticsRaw& statistics_raw)
{
    return PStatus::Error ();
}

// to_string call. Convert any content of the Noop enforcement object in string-based format.
std::string CompressionObject::to_string ()
{
    return std::string ("Compression enforcement object (")
        .append (std::to_string (this->m_object_id))
        .append (").\n");
}

} // namespace paio::enforcement
