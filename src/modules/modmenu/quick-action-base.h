#pragma once
#include "common.h"
#include "segment-base.h"

namespace ModMenuModule {
	class QuickActionBase {
	public:
		virtual ~QuickActionBase() = default;
		virtual void Execute() = 0;
		virtual const std::wstring& GetLabel() const = 0;

		virtual bool SetDataFromSegmentData(SegmentBase* segment) { return true; }
		virtual bool SetSegmentDataFromData(SegmentBase* segment) { return true; }
		virtual std::vector<uint8_t> SerializeData() const { return {}; }
		virtual bool DeserializeData(const std::vector<uint8_t>& data) { return true; }

	protected:
		QuickActionBase() = default;
		QuickActionBase(const QuickActionBase&) = delete;
		QuickActionBase& operator=(const QuickActionBase&) = delete;
	};

	template<typename SegmentDataT, typename DataT = SegmentDataT>
	class QuickActionWithSegment : public QuickActionBase {
	public:
		virtual const std::optional<DataT>& GetData() const final { return m_data; };
		virtual void SetData(const DataT& data) final {
			m_data = data;
			OnDataChange();
		}

		virtual bool SetDataFromSegmentData(SegmentBase* segment) override {
			Segment<SegmentDataT>* typedSegment = dynamic_cast<Segment<SegmentDataT>*>(segment);
			if (!typedSegment) {
				spdlog::error("QuickActionWithSegment::SetDataFromSegmentData: Invalid segment type");
				return false;
			}
			std::optional<SegmentDataT> segmentData = typedSegment->GetSegmentData();
			if (!segmentData.has_value()) {
				spdlog::error("QuickActionWithSegment::SetDataFromSegmentData: Segment returned no data");
				return false;
			}
			std::optional<DataT> data = SegmentDataToData(segmentData.value());
			if (!data.has_value()) {
				spdlog::error("QuickActionWithSegment::SetDataFromSegmentData: Failed to convert segment data to data");
				return false;
			}
			m_data = data.value();
			OnDataChange();
			return true;
		}

		virtual bool SetSegmentDataFromData(SegmentBase* segment) override {
			Segment<SegmentDataT>* typedSegment = dynamic_cast<Segment<SegmentDataT>*>(segment);
			if (!typedSegment) {
				spdlog::error("QuickActionWithSegment::SetSegmentDataFromData: Invalid segment type");
				return false;
			}
			if (!m_data.has_value()) {
				spdlog::error("QuickActionWithSegment::SetSegmentDataFromData: No data to set on segment");
				return false;
			}
			std::optional<SegmentDataT> segmentData = DataToSegmentData(m_data.value());
			if (!segmentData.has_value()) {
				spdlog::error("QuickActionWithSegment::SetSegmentDataFromData: Failed to convert data to segment data");
				return false;
			}
			return typedSegment->SetSegmentData(segmentData.value());
		}

		virtual std::optional<DataT> SegmentDataToData(const SegmentDataT& segmentData) {
			if constexpr (std::is_convertible<SegmentDataT, DataT>::value) {
				return static_cast<DataT>(segmentData);
			}
			else {
				return std::nullopt;
			}
		}

		virtual std::optional<SegmentDataT> DataToSegmentData(const DataT& data) {
			if constexpr (std::is_convertible<DataT, SegmentDataT>::value) {
				return static_cast<SegmentDataT>(data);
			}
			else {
				return std::nullopt;
			}
		}

		virtual std::vector<uint8_t> SerializeData() const override {
			if (!m_data.has_value()) {
				spdlog::error("QuickActionWithSegment::SerializeData: No data to serialize.");
				return {};
			}
			const DataT& data = m_data.value();
			std::vector<uint8_t> buffer;
			buffer.resize(sizeof(data));
			std::memcpy(buffer.data(), &data, sizeof(data));
			return buffer;
		}

		virtual bool DeserializeData(const std::vector<uint8_t>& data) override {
			const std::vector<uint8_t>* dataPtr = &data;
			std::vector<uint8_t> dataCopy;
			if (dataPtr->size() != sizeof(DataT)) {
				dataCopy = *dataPtr;
				if (!HandleInvalidDataSize(dataCopy)) {
					spdlog::error("QuickActionWithSegment::DeserializeData: Unhandled invalid data size. Expected {}, got {}.", sizeof(DataT), dataPtr->size());
					return false;
				}
				dataPtr = &dataCopy;

				if (dataPtr->size() != sizeof(DataT)) {
					spdlog::error("QuickActionWithSegment::DeserializeData: Invalid data size after handling. Expected {}, got {}.", sizeof(DataT), dataPtr->size());
					return false;
				}
			}
			DataT dataValue;
			std::memcpy(&dataValue, dataPtr->data(), sizeof(DataT));
			m_data = dataValue;
			OnDataChange();
			return true;
		}

		virtual bool HandleInvalidDataSize(std::vector<uint8_t>& data) {
			return false;
		}

	protected:
		QuickActionWithSegment() = default;

		virtual void OnDataChange() {}

		std::optional<DataT> m_data;
	};
}
