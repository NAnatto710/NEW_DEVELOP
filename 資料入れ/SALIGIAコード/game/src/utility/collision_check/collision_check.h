
/*!
 *  @file       collison_check.h
 *  @brief      “–‚½‚è”»’è
 *  @author     Ryusei Shimizu
 *  @date       2026/03/20
 */

#pragma once

#include "vivid.h"

/*!
 *  @brief      “_Œ^‚Ì\‘¢‘Ì
 */
struct Point
{
	vivid::Vector2 Position;
};

/*!
 *  @brief      ü•ªŒ^‚Ì\‘¢‘Ì
 */
struct Segment
{
	vivid::Vector2 Start;
	vivid::Vector2 End;
};

/*!
 *  @brief      ‰~Œ^‚Ì\‘¢‘Ì
 */
struct Circle
{
	vivid::Vector2 Center;
	float Radius;
};

/*!
 *  @brief      ’·‰~Œ^‚Ì\‘¢‘Ì
 */
struct Capsule
{
	vivid::Vector2 Start;
	vivid::Vector2 End;
	float Radius;
};

/*!
 *  @brief      ’·•ûŒ`Œ^‚Ì\‘¢‘Ì
 */
struct AABB
{
	vivid::Vector2 Position;   // ¶ã
	float Width;
	float Height;
};

/*!
 *  @brief      ‰ñ“]’·•ûŒ`Œ^‚Ì\‘¢‘Ì
 */
struct OBB
{
	vivid::Vector2 Center;
	vivid::Vector2 HalfSize;
	float Rotation;
};

namespace IsCollision
{
	/*!
	 *  @brief      “_“¯m‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  point1      “_1
	 *  @param[in]  point2      “_2
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Point& point1, const Point& point2);
	
	/*!
	 *  @brief      “_‚Æü•ª‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  point       “_
	 *  @param[in]  segment     ü•ª
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Point& point, const Segment& segment);
	
	/*!
	 *  @brief      “_‚Æ‰~‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  point       “_
	 *  @param[in]  circle      ‰~
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Point& point, const Circle& circle);
	
	/*!
	 *  @brief      “_‚Æ’·‰~‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  point       “_
	 *  @param[in]  capsule     ’·‰~
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Point& point, const Capsule& capsule);
	
	/*!
	 *  @brief      “_‚Æ’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  point       “_
	 *  @param[in]  aabb        ’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Point& point, const AABB& aabb);
	
	/*!
	 *  @brief      “_‚Æ‰ñ“]’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  point       “_
	 *  @param[in]  obb         ‰ñ“]’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Point& point, const OBB& obb);

	// ü•ª‚Ì“–‚½‚è”»’è

	/*!
	 *  @brief      ü•ª“¯m‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  segment1    ü•ª1
	 *  @param[in]  segment2    ü•ª2
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Segment& segment1, const Segment& segment2);

	/*!
	 *  @brief      ü•ª‚Æ‰~‚Ì“–‚½‚è”»’è
	 * 
	 *	@param[in]  segment     ü•ª
	 *	@param[in]  circle      ‰~
	 * 
	 *	@return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Segment& segment, const Circle& circle);

	/*!
	 *  @brief      ü•ª‚Æ’·‰~‚Ì“–‚½‚è”»’è
	 * 
	 *	@param[in]  segment     ü•ª
	 *	@param[in]  capsule     ’·‰~
	 * 
	 *	@return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Segment& segment, const Capsule& capsule);

	/*!
	 *  @brief      ü•ª‚Æ’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  segment     ü•ª
	 *  @param[in]  aabb        ’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Segment& segment, const AABB& aabb);

	/*!
	 *  @brief      ü•ª‚Æ‰ñ“]’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  segment     ü•ª
	 *  @param[in]  obb         ‰ñ“]’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Segment& segment, const OBB& obb);

	/*!
	 *  @brief      ‰~“¯m‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  circle1     ‰~1
	 *  @param[in]  circle2     ‰~2
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Circle& circle1, const Circle& circle2);

	/*!
	 *  @brief      ‰~‚Æ’·‰~‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  circle      ‰~
	 *  @param[in]  capsule     ’·‰~
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Circle& circle, const Capsule& capsule);

	/*!
	 *	@brief      ‰~‚Æ’·•ûŒ`‚Ì“–‚½‚è”»’è
	 * 
	 *	@param[in]  circle      ‰~
	 *	@param[in]  aabb        ’·•ûŒ`
	 * 
	 *	@return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Circle& circle, const AABB& aabb);

	/*!
	 *	@brief      ‰~‚Æ‰ñ“]’·•ûŒ`‚Ì“–‚½‚è”»’è
	 * 
	 *	@param[in]  circle      ‰~
	 *	@param[in]  obb         ‰ñ“]’·•ûŒ`
	 * 
	 *	@return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Circle& circle, const OBB& obb);

	/*!
	 *  @brief      ’·‰~“¯m‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  capsule1    ’·‰~1
	 *  @param[in]  capsule2    ’·‰~2
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Capsule& capsule1, const Capsule& capsule2);

	/*!
	 *  @brief      ’·‰~‚Æ’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  capsule     ’·‰~
	 *  @param[in]  aabb        ’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Capsule& capsule, const AABB& aabb);

	/*!
	 *  @brief      ’·‰~‚Æ‰ñ“]’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  capsule     ’·‰~
	 *  @param[in]  obb         ‰ñ“]’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const Capsule& capsule, const OBB& obb);

	/*!
	 *  @brief      ’·•ûŒ`“¯m‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  aabb1       ’·•ûŒ`1
	 *  @param[in]  aabb2       ’·•ûŒ`2
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const AABB& aabb1, const AABB& aabb2);

	/*!
	 *  @brief      ’·•ûŒ`‚Æ‰ñ“]’·•ûŒ`‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  aabb        ’·•ûŒ`
	 *  @param[in]  obb         ‰ñ“]’·•ûŒ`
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const AABB& aabb, const OBB& obb);

	/*!
	 *  @brief      ‰ñ“]’·•ûŒ`“¯m‚Ì“–‚½‚è”»’è
	 *
	 *  @param[in]  obb1        ‰ñ“]’·•ûŒ`1
	 *  @param[in]  obb2        ‰ñ“]’·•ûŒ`2
	 *
	 *  @return     “–‚½‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©
	 */
	bool IsHit(const OBB& obb1, const OBB& obb2);
}