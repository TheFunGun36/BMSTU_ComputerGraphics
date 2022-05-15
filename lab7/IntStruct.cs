using Godot;
using System;

public struct Vector2Int
{
	public int x;
	public int y;

	public Vector2 ToVector2() => new Vector2(x, y);

	public Vector2Int(int x, int y)
	{
		this.x = x;
		this.y = y;
	}
	public Vector2Int(Vector2 v)
	{
		this.x = (int)v.x;
		this.y = (int)v.y;
	}

	public static Vector2Int operator +(Vector2Int a, Vector2Int b)
	{
		a.x += b.x;
		a.y += b.y;
		return a;
	}
	public static Vector2Int operator -(Vector2Int a, Vector2Int b)
	{
		a.x -= b.x;
		a.y -= b.y;
		return a;
	}
}

public struct Rect2Int
{
	public Vector2Int topLeft;
	public Vector2Int bottomRight;
	public int Left
	{
		get => topLeft.x;
		set => topLeft.x = value;
	}
	public int Right
	{
		get => bottomRight.x;
		set => bottomRight.x = value;
	}
	public int Top
	{
		get => topLeft.y;
		set => topLeft.y = value;
	}
	public int Bottom
	{
		get => bottomRight.y;
		set => bottomRight.y = value;
	}

	public Rect2 ToRect2()
	{
		Vector2 Size = new Vector2(Right - Left + 1, Bottom - Top + 1);
		return new Rect2(topLeft.ToVector2(), Size);
	}
	public void Abs()
	{
		if (Left > Right)
			(Right, Left) = (Left, Right);
		if (Top > Bottom)
			(Top, Bottom) = (Bottom, Top);
	}

	public Rect2Int(Vector2Int topLeft, Vector2Int bottomRight)
	{
		this.topLeft = topLeft;
		this.bottomRight = bottomRight;
	}
	public Rect2Int(Rect2 r)
	{
		topLeft = new Vector2Int(r.Position);
		bottomRight = new Vector2Int(r.Position + r.Size);
		bottomRight.x--;
		bottomRight.y--;
	}
}
