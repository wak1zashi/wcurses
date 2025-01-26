#pragma once

#include "Point.h"

namespace __internal
{
    class Cursor
    {
    private:
        const short _MIN_CURSOR = 0; 
        const short _MIN_RANGE  = 1; 

        waki::Point _cursor;
        waki::Point _range;  
    public:
    	// ====================================================================
	    // =========================== Constructors ===========================
        // ====================================================================

        Cursor();

        Cursor(short maxY, short maxX);
        Cursor(const waki::Point& range);

		Cursor(short y, short x, short maxY, short maxX);
		Cursor(const waki::Point& cursor, const waki::Point& range);

        // ====================================================================
        // =================== Coordinate Checking Methods ====================
        // ====================================================================

        bool isPointInRange(short y, short x) const;

        // ====================================================================
	    // ====================== Stepwise Cursor Adjustment =======================
        // ====================================================================

        void incrementY();
        void incrementX();

        void decrementY();
        void decrementX();

    	// ====================================================================
	    // ===================== Methods For Moving Cursor ====================
        // ====================================================================

        void reset();
        void resetY();
        void resetX();

        bool move(short y, short x);
	    bool move(const waki::Point& cursor);
  
        bool moveBy(short dy, short dx);
	    bool moveBy(const waki::Point& dCursor);
		
        bool moveY(short y);
		bool moveX(short x);

    	// ====================================================================
	    // ===================== Cursor Limits Management =====================
        // ====================================================================

        bool setRange(short maxY, short maxX);
        bool setRange(const waki::Point& range);

        // ====================================================================
	    // ==================== Boundary Checking Methods =====================
        // ====================================================================

        bool isAtTop() const    { return _cursor.y == _MIN_CURSOR; }
        bool isAtBottom() const { return _cursor.y == _range.y - 1; }
        bool isAtLeft() const   { return _cursor.x == _MIN_CURSOR; }
        bool isAtRight() const  { return _cursor.x == _range.x - 1; }

    	// ====================================================================
	    // ========================== Getter Methods ==========================
        // ====================================================================

        const waki::Point& getYX() const { return _cursor; } 
        const short& getY() const { return _cursor.y; } 
        const short& getX() const { return _cursor.x; } 

        const waki::Point& getRange() const { return _range; }
        const short& getMaxY() const { return _range.y; } 
        const short& getMaxX() const { return _range.x; } 
   };
} // namespace __internal