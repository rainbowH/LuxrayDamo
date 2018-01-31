#pragma once
#include <map>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <qlist.h>
#include <qvector2d.h>
#include <qvector.h>
#include <qmap.h>

#include "Vertex.h"
#include "Polar.h"
#include "Range.h"
#include "OpenglPointMsg.h"
//NJ840-2L
//NJ840-4L
//SH13LA
//SH840-4L
//SH840-135
namespace Wheel {
	class WheelSpreadParm
	{
	public:
		explicit WheelSpreadParm() :unitWidth(0), unitLength(0), lengthRange(), widthRange(), spreadYRange(), standardTreadMsg() {}
		explicit WheelSpreadParm(/*float wheelR,*/float unitWidth, float unitLength, const Range& lengthRange, const Range& widthRange, const Range& spreadYRange, const std::vector<glm::vec2>& standardTreadMsg)
			:/*wheelR(wheelR),*/unitWidth(unitWidth), unitLength(unitLength), lengthRange(lengthRange), widthRange(widthRange), spreadYRange(spreadYRange), standardTreadMsg(standardTreadMsg) {}
		//float GetWheelR() const
		//{
		//	return wheelR;
		//}
		float GetUnitWidth() const
		{
			return unitWidth;
		}
		float GetUnitLength() const
		{
			return unitLength;
		}
		Range GetLengthRange() const
		{
			return lengthRange;
		}
		Range GetWidthRange() const
		{
			return widthRange;
		}
		Range GetSpreadYRange() const
		{
			return spreadYRange;
		}
		const std::vector<glm::vec2>& GetStandardTreadMsg() const
		{
			return standardTreadMsg;
		}
	private:
		//float wheelR;
		//float originHeight;
		float unitWidth;
		float unitLength;
		Range lengthRange;
		Range widthRange;
		Range spreadYRange;
		std::vector<glm::vec2> standardTreadMsg;
	};

	struct WheelSpreadMsg
	{
		OpenglObj::OpenglPointMsg wheelMsg;
		WheelSpreadParm wheelSpreadParm;
	};


	class WheelFlangeTread
	{
	public:
		static const float PIR;
		static const float FITBEGINNODE;//从32开始拟合，32之前的不变,一直拟合到surveyTreadBegin
		static const float FITENDNODE;//从surveyTreadEnd拟合到108.0f结束
		static const float SURVEYTREADBEGIN;
		static const float SURVEYTREADEND;
		static const float STANDARDTREADRADIUS;
		static const float STANDARDWIDTH;
		static const float CIRCUMFERENCE;
		static const int kFloatPlantSection;

	public:
		//Wheel(/*const std::string &path,const std::string &type*/);
		WheelFlangeTread(const QVector<QVector2D>& originSurveyFlange, const QMap<float, QVector<float>>& surveyTread/*, const QString type = "NJ840-2L"*/);
		OpenglObj::OpenglPointMsg LoadFlangeTreadPoint(/*std::map<float, std::vector<Polar>> &flangeTread, *//*std::vector<QVector2D> &floatPlant*/);//点的生成（包含与浮板的拼接）
		WheelSpreadMsg LoadSpreadPoint();
		Range GetTreadRange() const;
		QVector<QVector2D> GetSection(float radian);
		std::vector<glm::vec2> GetStandardTreadMsg() const;
		//const std::vector<glm::vec2>& GetAverRadius() const;
		const std::map<float, std::vector<Polar>>& get_flange_tread();
		~WheelFlangeTread();
		//const std::vector<Vertex>& GetVertices() const;
		//const std::vector<unsigned int>& GetIndices() const;
	private:
		//float GetAverRadius(float plie, float radian, std::map<float, std::vector<Polar>>& sampleData);
		//void InitStandardTreadData(std::map<float, std::vector<Polar>>&standardTread/*const std::string& type*/);
		//void InitSurveyTreadData(std::map<float, std::vector<Polar>>& surveyTread);
		void InitSurveyFlange(QVector<QVector2D>& surveyFlange);
		//void InitFloatPlateData(/*std::vector<QVector2D> &floatPlant, */QString type);
		void InitFlangeTread(/*std::map<float, std::vector<Polar>> &flangeTread,*/const QVector<QVector2D>& surveyFlange, const QMap<float, QVector<float>>& surveyTread, const std::vector<QVector2D> & standardOutsideTread);
		void InitStandardOutsideTread(std::vector<QVector2D> &standardOutsideTread);
		//void InitAverRadius(const QMap<float, QVector<float>>& surveyTread);
		//void InitStandardOtherData(std::vector<glm::vec2>& otherData);
		//std::vector<Vertex> vertices;
		//std::vector<unsigned int> indices;
		std::map<float, std::vector<Polar>> flangeTread;
		//std::vector<QVector2D> floatPlant;
		//std::vector<glm::vec2> averRadius;

		//void LoadPoint(std::map<float, std::vector<Polar>>& sampleData, std::map<float, std::vector<Polar>>& standardDrawData, std::vector<glm::vec2>& otherData);
		//void ConvertToPolar(float *)
		Range surveyTreadRange;
		//float surveyTreadStart;
		//float surveyTreadEnd;
		//float minFlangeTreadY;

		//wheelSpread
	};
	std::vector<glm::vec2> InitFloatPlateData(QString filePath);
	OpenglObj::OpenglPointMsg LoadFloatPlantPoint(const std::vector<glm::vec2>& floatPlant);

	OpenglObj::OpenglPointMsg LoadConnectionPoint(const std::map<float,std::vector<Polar>>& flangeTread,const std::vector<glm::vec2> & floatPlant);
}