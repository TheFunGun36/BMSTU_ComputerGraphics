using Godot;
using System;

public class Rect : HBoxContainer
{
	SpinBox _spinX1;
	SpinBox _spinY1;
	SpinBox _spinX2;
	SpinBox _spinY2;
	Button _buttonDelete;

	[Signal] delegate void Changed();
	[Export] NodePath SpinX1Path;
	[Export] NodePath SpinY1Path;
	[Export] NodePath SpinX2Path;
	[Export] NodePath SpinY2Path;

	public int X1
	{
		get => (int)_spinX1.Value;
		set
		{
			_spinX1.Value = value;
			_spinX2.MinValue = value;
		}
	}
	public int Y1
	{
		get => (int)_spinY1.Value;
		set
		{
			_spinY1.Value = value;
			_spinY2.MinValue = value;
		}
	}
	public int X2
	{
		get => (int)_spinX2.Value;
		set => _spinX2.Value = value;
	}
	public int Y2
	{
		get => (int)_spinY2.Value;
		set => _spinY2.Value = value;
	}

	public Rect2Int RectInt
	{
		get => new Rect2Int(new Vector2Int(X1, Y1), new Vector2Int(X2, Y2));
		set
		{
			X1 = value.Left;
			Y1 = value.Top;
			X2 = value.Right;
			Y2 = value.Bottom;
		}
	}

	public Rect2 RectFloat
	{
		get => new Rect2(new Vector2(X1, Y1), new Vector2(X2 - X1 + 1, Y2 - Y1 + 1));
		set
		{
			X1 = (int)value.Position.x;
			Y1 = (int)value.Position.y;
			X2 = (int)(value.Position.x + value.Size.x) - 1;
			Y2 = (int)(value.Position.x + value.Size.y) - 1;
		}
	}

	public override void _Ready()
	{
		_spinX1 = GetNode<SpinBox>(SpinX1Path);
		_spinY1 = GetNode<SpinBox>(SpinY1Path);
		_spinX2 = GetNode<SpinBox>(SpinX2Path);
		_spinY2 = GetNode<SpinBox>(SpinY2Path);
	}

	public void OnChanged(float value)
	{
		_spinX2.MinValue = X1;
		_spinY2.MinValue = Y1;
		EmitSignal("Changed");
	}
}
