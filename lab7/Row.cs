using Godot;
using System;

public class Row : HBoxContainer
{
	SpinBox _spinX1;
	SpinBox _spinY1;
	SpinBox _spinX2;
	SpinBox _spinY2;
	Button _buttonDelete;

	[Signal] delegate void Changed();
	[Signal] delegate void Destroy(int index);
	[Export] NodePath SpinX1Path;
	[Export] NodePath SpinY1Path;
	[Export] NodePath SpinX2Path;
	[Export] NodePath SpinY2Path;
	[Export] NodePath ButtonDeletePath;

	public int Index { get; set; }
	public int X1
	{
		get => (int)_spinX1.Value;
		set => _spinX1.Value = value;
	}
	public int Y1
	{
		get => (int)_spinY1.Value;
		set => _spinY1.Value = value;
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

	public Row()
	{
	}

	public override void _Ready()
	{
		_spinX1 = GetNode<SpinBox>(SpinX1Path);
		_spinY1 = GetNode<SpinBox>(SpinY1Path);
		_spinX2 = GetNode<SpinBox>(SpinX2Path);
		_spinY2 = GetNode<SpinBox>(SpinY2Path);
		_buttonDelete = GetNode<Button>(ButtonDeletePath);
	}

	public void OnChanged(float f)
		=> EmitSignal("Changed");

	public void OnDestroy()
		=> EmitSignal("Destroy", Index);
}
