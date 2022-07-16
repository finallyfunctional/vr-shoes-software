package com.finallyfunctional.vr_shoes.communication.observers;

import com.finallyfunctional.vr_shoes.VrShoe;
import com.finallyfunctional.vr_shoes.communication.commands.ShoeConfiguration;

public interface IShoeConfigurationsObserver
{
    void shoeConfigurationsRead(ShoeConfiguration message, VrShoe vrShoe);
}
