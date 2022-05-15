using Godot;
using System;
using System.Collections.Generic;

public class Canvas : TextureRect
{
	public bool StateDrawEllipse { get; set; } = false;
	private Image _pixmap = new Image();
	private bool _isInput = false;
	private bool _shift = false;
	private bool _filling = false;
	private Vector2 _previousPoint = new Vector2();
	private Vector2 _fillBeginPoint = new Vector2();
	private IEnumerator<bool> _delayIterator;
	private LinkedList<Vector2> _points = new LinkedList<Vector2>();
	private Vector2 _currentPosition;

	[Export] public uint AdvancesPerTick = 1;
	[Export] public Color BackgroundColor { get; set; }
	[Export] public Color ActiveColor { get; set; }
	[Export] public Color MiscColor { get; set; }
	[Export] private NodePath ButtonFillDelay { get; set; }
	[Export] private NodePath ButtonFill { get; set; }
	[Export] private NodePath ButtonClear { get; set; }
	[Export] private NodePath SliderSpeed { get; set; }

	///// INITIALIZATION /////
	public override void _Ready()
	{
		_previousPoint = RectSize;
		ConnectAllDependencies();
		CreatePixmap();
		CreateTexture();
		ClearPixmap();
	}
	private void ConnectAllDependencies()
	{
		GetNode(ButtonFill).Connect("pressed", this, "FillArea");
		GetNode(ButtonFillDelay).Connect("pressed", this, "StartFillAreaDelayed");
		GetNode(ButtonClear).Connect("pressed", this, "ClearPixmap");
		GetNode(SliderSpeed).Connect("value_changed", this, "AdvancesPerTickNew");
	}
	private void CreatePixmap()
	{
		Rect2 r = GetViewportRect();
		_pixmap.Create((int)r.Size.y, (int)r.Size.y, false, Image.Format.Rgba8);
	}
	private void CreateTexture()
	{
		Texture = new ImageTexture();
		(Texture as ImageTexture).CreateFromImage(_pixmap);
	}
	private void ClearPixmap()
	{
		_pixmap.Fill(BackgroundColor);
		_delayIterator = null;
		_filling = false;
		UpdateImage();
	}
	private void UpdateImage() => (Texture as ImageTexture).SetData(_pixmap);
	static private Vector2 vectorInc(Vector2 vec)
	{
		return new Vector2(vec.x + 1, vec.y + 1);
	}

	///// DRAW GODOT /////
	public override void _Draw()
	{
		base._Draw();
		if (_isInput)
		{
			if (!StateDrawEllipse)
			{
				Vector2 to;
				Vector2 previousPoint = _points.First.Value;
				for (var node = _points.First.Next; node != null; node = node.Next)
				{
					to = node.Value;
					DrawLine(vectorInc(previousPoint), vectorInc(node.Value), ActiveColor);
					previousPoint = node.Value;
				}

				to = GetLocalMousePosition();
				to = _shift ? ToStraightLine(to) : to;
				DrawLine(vectorInc(_previousPoint), vectorInc(to), ActiveColor);
			}
			else
			{
				Vector2 mouse = GetLocalMousePosition();
				Vector2 size = new Vector2(mouse.x - _previousPoint.x, mouse.y - _previousPoint.y);
				Rect2 r = new Rect2(_previousPoint, size);
				DrawRect(r, ActiveColor, false);
			}
		}

		if (!_filling)
			DrawCross(_fillBeginPoint, 15, MiscColor);
		else
			DrawCross(_currentPosition, 5, MiscColor);
		UpdateImage();
	}
	private void DrawCross(Vector2 point, int size, Color color)
	{
		Vector2 up = new Vector2(point.x + 1, point.y - size + 1);
		Vector2 down = new Vector2(point.x + 1, point.y + size + 1);
		Vector2 left = new Vector2(point.x - size + 1, point.y + 1);
		Vector2 right = new Vector2(point.x + size + 1, point.y + 1);
		DrawLine(up, down, color);
		DrawLine(left, right, color);
	}

	///// DELAY /////
	public override void _Process(float delta)
	{
		if (_filling)
		{
			for (var i = 0; _filling && i < AdvancesPerTick; i++)
			{
				_delayIterator.MoveNext();
				_filling = _delayIterator.Current;
			}
			UpdateImage();
			Update();
		}
	}

	///// INPUT HANDLE /////
	public override void _Input(InputEvent @event)
	{
		base._Input(@event);

		if (@event is InputEventMouseButton)
			HandleMouseButtonEvent(@event as InputEventMouseButton);
		else if (@event is InputEventMouseMotion && _isInput)
			Update();
		else if (@event is InputEventKey)
			HandleKeyButtonEvent(@event as InputEventKey);
	}
	private void HandleKeyButtonEvent(InputEventKey @event)
	{
		if (@event.Scancode == (uint)KeyList.Shift)
		{
			_shift = @event.Pressed;
			Update();
		}
	}
	private void HandleMouseButtonEvent(InputEventMouseButton @event)
	{
		if (@event.Pressed && GetRect().HasPoint(@event.Position))
		{
			if (@event.ButtonIndex == (int)ButtonList.Left)
			{
				if (@event.Control)
					PlaceFillPoint(@event.Position - RectPosition);
				else
					InputNode(@event.Position - RectPosition, @event.Shift);
			}
			else if (@event.ButtonIndex == (int)ButtonList.Right)
				InputEnd();
		}
	}
	private void InputNode(Vector2 position, bool straight)
	{
		if (straight && _isInput)
			_previousPoint = StateDrawEllipse ? ToCircle(position) : ToStraightLine(position);
		else
			_previousPoint = position;

		if (StateDrawEllipse && _points.Count > 0)
			InputEnd();
		else
		{
			_points.AddLast(_previousPoint);
			_isInput = true;
		}
	}
	private void InputEnd()
	{
		if (StateDrawEllipse && _points.Count > 0)
			ApplyInputCircle();
		else if (_points.Count > 2)
			ApplyInputLines();

		UpdateImage();
		_points.Clear();
		_isInput = false;
		Update();
	}
	private void ApplyInputCircle()
	{
		_pixmap.Lock();
		DrawEllipseAveragePoint(_points.Last.Value, _previousPoint, ActiveColor);
		_pixmap.Unlock();
	}
	private void ApplyInputLines()
	{
		_pixmap.Lock();
		Vector2 previousPoint = _points.First.Value;
		for (var node = _points.First.Next; node != null; node = node.Next)
		{
			DrawLineDDA(previousPoint, node.Value, ActiveColor);
			previousPoint = node.Value;
		}
		DrawLineDDA(_points.Last.Value, _points.First.Value, ActiveColor);
		_pixmap.Unlock();
	}
	private void PlaceFillPoint(Vector2 position)
	{
		_fillBeginPoint = position;
		Update();
	}
	private Vector2 ToStraightLine(Vector2 point)
	{
		if (Math.Abs(point.y - _previousPoint.y) >= Math.Abs(point.x - _previousPoint.x))
			point.x = _previousPoint.x;
		else
			point.y = _previousPoint.y;

		return point;
	}
	private Vector2 ToCircle(Vector2 point)
	{
		float dx = Math.Abs(point.x - _previousPoint.x);
		float dy = Math.Abs(point.y - _previousPoint.y);

		if (dx > dy)
			point.x = _previousPoint.x + dx;
		else
			point.y = _previousPoint.y + dy;

		return point;
	}

	///// PIXEL BY PIXEL DRAW /////
	private void DrawLineDDA(Vector2 p1, Vector2 p2, Color color)
	{
		// calculate dx , dy
		int dx = (int)(p2.x - p1.x);
		int dy = (int)(p2.y - p1.y);

		// Depending upon absolute value of dx & dy
		// choose number of steps to put pixel as
		// steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy)
		int steps = Math.Abs(dx) > Math.Abs(dy) ? Math.Abs(dx) : Math.Abs(dy);

		// calculate increment in x & y for each steps
		float incX = dx / (float)steps;
		float incY = dy / (float)steps;

		// Put pixel for each step
		float x = p1.x;
		float y = p1.y;
		for (int i = 0; i <= steps; i++)
		{
			//putpixel(round(X), round(Y), WHITE);
			_pixmap.SetPixel((int)x, (int)y, color);
			x += incX;
			y += incY;
		}
	}
	private void DrawEllipseAveragePoint(Vector2 p1, Vector2 p2, Color color)
	{
		double rx = Math.Abs(p1.x - p2.x) / 2.0;
		double ry = Math.Abs(p1.y - p2.y) / 2.0;
		double cx = (p1.x + p2.x) / 2.0;
		double cy = (p1.y + p2.y) / 2.0;
		double dx, dy, d1, d2, x, y;
		x = 0;
		y = ry;

		// Initial decision parameter of region 1
		d1 = ry * ry - rx * rx * ry + 0.25f * rx * rx;
		dx = 2 * ry * ry * x;
		dy = 2 * rx * rx * y;

		// For region 1
		while (dx < dy)
		{
			// Print points based on 4-way symmetry
			_pixmap.SetPixel((int)(cx + x), (int)(cy + y), color);
			_pixmap.SetPixel((int)(cx + x), (int)(cy - y), color);
			_pixmap.SetPixel((int)(cx - x), (int)(cy - y), color);
			_pixmap.SetPixel((int)(cx - x), (int)(cy + y), color);

			// Checking and updating value of
			// decision parameter based on algorithm
			if (d1 < 0)
			{
				x++;
				dx = dx + 2 * ry * ry;
				d1 = d1 + dx + ry * ry;
			}
			else
			{
				x++;
				y--;
				dx = dx + 2 * ry * ry;
				dy = dy - 2 * rx * rx;
				d1 = d1 + dx - dy + ry * ry;
			}
		}

		// Decision parameter of region 2
		d2 = ry * ry * ((x + 0.5f) * (x + 0.5f))
			+ rx * rx * ((y - 1) * (y - 1))
			- rx * rx * ry * ry;

		// Plotting points of region 2
		while (y >= 0)
		{
			// Print points based on 4-way symmetry
			_pixmap.SetPixel((int)(cx + x), (int)(cy + y), color);
			_pixmap.SetPixel((int)(cx + x), (int)(cy - y), color);
			_pixmap.SetPixel((int)(cx - x), (int)(cy - y), color);
			_pixmap.SetPixel((int)(cx - x), (int)(cy + y), color);

			// Checking and updating parameter
			// value based on algorithm
			if (d2 > 0)
			{
				y--;
				dy = dy - 2 * rx * rx;
				d2 = d2 + rx * rx - dy;
			}
			else
			{
				y--;
				x++;
				dx = dx + 2 * ry * ry;
				dy = dy - 2 * rx * rx;
				d2 = d2 + dx - dy + rx * rx;
			}
		}
	}
	private void FillArea()
	{
		_delayIterator = FillAreaDelayed();
		do _delayIterator.MoveNext(); while (_delayIterator.Current);
		UpdateImage();
	}
	private void StartFillAreaDelayed()
	{
		_filling = true;
		_delayIterator = FillAreaDelayed();
	}
	private IEnumerator<bool> FillAreaDelayed()
	{
		Stack<Vector2> points = new Stack<Vector2>();
		points.Push(_fillBeginPoint);

		while (points.Count > 0)
		{
			Vector2 point = points.Pop();
			_currentPosition = point;
			int x = (int)point.x;
			int xLeft;
			int xRight;
			int y = (int)point.y;
			_pixmap.Lock();
			_pixmap.SetPixel(x, y, ActiveColor);
			int tx = x;

			x--;
			Color clr = _pixmap.GetPixel(x, y);
			while (x > 0 && clr == BackgroundColor)
			{
				_pixmap.SetPixel(x, y, ActiveColor);
				x--;
				clr = _pixmap.GetPixel(x, y);
			}
			xLeft = x + 1;

			x = tx + 1;
			clr = _pixmap.GetPixel(x, y);
			while (x < RectSize.x && clr == BackgroundColor)
			{
				_pixmap.SetPixel(x, y, ActiveColor);
				x++;
				clr = _pixmap.GetPixel(x, y);
			}
			xRight = x - 1;
			_pixmap.Unlock();

			yield return true;

			_pixmap.Lock();
			if (y > 0)
			{
				y--;
				FindNewPoints(points, xLeft, xRight, x, y);
				y++;
			}
			if (y < RectSize.y - 1)
			{
				y++;
				FindNewPoints(points, xLeft, xRight, x, y);
				y--;
			}
			_pixmap.Unlock();
		}

		yield return false;
	}
	private void FindNewPoints(Stack<Vector2> points, int xLeft, int xRight, int x, int y)
	{
		x = xLeft;
		while (x <= xRight)
		{
			bool fl = false;
			while (_pixmap.GetPixel(x, y) == BackgroundColor && x <= xRight)
			{
				fl = true;
				x++;
			}

			if (fl)
			{
				if (_pixmap.GetPixel(x, y) == BackgroundColor && x < xRight)
					points.Push(new Vector2(x, y));
				else
					points.Push(new Vector2(x - 1, y));
			}

			int xn = x;
			while (_pixmap.GetPixel(x, y) != BackgroundColor && x < xRight)
				x++;

			if (x == xn)
				x++;
		}
	}
	
	///// SLOTS /////
	private void AdvancesPerTickNew(int value) => AdvancesPerTick = (uint)value;
}
