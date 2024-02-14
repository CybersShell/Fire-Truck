enum leftStickStates {fireTruckRight, fireTruckLeft, fireTruckStraight};
struct LeftStickState
{
    leftStickStates newState;
    leftStickStates oldState;
} leftStick;

enum fireTruckStates
{
    forward,
    backward,
    right,
    left,
    still,
    forwardRight,
    forwardLeft,
    backwardLeft,
    backwardRight,
};

struct fireTruckState
{
    fireTruckStates newState;
    fireTruckStates oldState;
} fireTruck;

void getState()
{
    leftStick.newState = getStateOfLeftStick();
    rightStick.newState = getStateOfRightStick();
    combinedState = combineStates(); // will control what gets sent to slave
    sendData();
    leftStick.oldState = leftStick.newState;
    rightStick.oldState = rightStick.newState;
}

void combineStates()
{
    if (leftStick.newState != leftStick.oldState)
    {
        // leftStick state is different
        switch (leftStick.newState)
        {
        case leftStickStates::fireTruckLeft:
            fireTruck.newState = fireTruckStates::left;
            break;
        case leftStickStates::fireTruckRight:
            fireTruck.newState = fireTruckStates::right;
            break;
        default:
            break;
        }
    }
}

leftStickStates getStateOfLeftStick()
{
    bool isLeft = leftConditional;
    bool isRight = rightConditional;
    bool isNeutral = !isLeft && !isRight;

    // state is different
    if (leftStick.oldState != leftStick.newState)
    {
        if (isNeutral) // state is different
        {
            return leftStickStates::fireTruckStraight;
        }
        else if (isRight)
        {
            return leftStickStates::fireTruckRight;
        }
        return leftStickStates::fireTruckLeft;
    }
}