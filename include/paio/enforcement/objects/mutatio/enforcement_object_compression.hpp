/**
 *   Written by Pedro Peixoto. 
 **/

#ifndef PAIO_ENFORCEMENT_OBJECT_COMPRESSION_HPP
#define PAIO_ENFORCEMENT_OBJECT_COMPRESSION_HPP

#include <paio/enforcement/objects/enforcement_object.hpp>
#include <paio/utils/logging.hpp>

namespace paio::enforcement {

class CompressionObject : public EnforcementObject {

private:
    long m_object_id { 0 };

public:
    /**
     * CompressionObject default constructor.
     */
    CompressionObject ();

    /**
     * CompressionObject parameterized constructor.
     * @param object_id EnforcementObject identifier.
     */
    explicit CompressionObject (const long& object_id);

    /**
     * CompressionObject parameterized constructor.
     * @param tag_name Name of the compression object.
     */
    CompressionObject (const long& object_id, const std::string& tag_name);

    /**
     * CompressionObject default destructor.
     */
    ~CompressionObject () override final;

    /**
     * get_enforcement_object_id: Get the EnforcementObject's identifier.
     * @return Returns a copy of the m_object_id value.
     */
    [[nodiscard]] long get_enforcement_object_id () const override final;

    /**
     * obj_enforce: apply the Compression enforcement mechanisms over requests.
     * In this case, it compresses the request.
     * @param ticket Represents a metadata-like object that contains a set of elements that
     * characterize the request, including the request context, size, and buffer.
     * @param result Reference to a Result object that stores the result of Compression enforcement
     * mechanism, including the request content if enabled.
     */
    void obj_enforce (const Ticket& ticket, Result& result) override final;

    /**
     * obj_configure: configure internal properties of the Compression enforcement object.
     * @param config Configuration property to be set/updated.
     * @param configuration_values Vector of configuration values to be updated.
     * @return PStatus state, being PStatus::OK if the configuration was successfully made, and
     * PStatus::ERROR otherwise.
     */
    PStatus obj_configure (int config, const std::vector<long>& configuration_values) override final;

    /**
     * obj_collect_statistics: collect and submit statistics from the Compression enforcement object to
     * the SDS control plane (controller).
     * @return PStatus state, being PStatus::OK if statistics were effectively collected, and
     * PStatus::Error otherwise.
     */
    PStatus obj_collect_statistics (ObjectStatisticsRaw& statistics_raw) override final;

    /**
     * to_string: generate a string-based format of the contents of the Compression enforcement object.
     * @return String of the Compression settings.
     */
    std::string to_string () override final;
};
} // namespace paio::enforcement

#endif // PAIO_ENFORCEMENT_OBJECT_COMPRESSION_HPP
