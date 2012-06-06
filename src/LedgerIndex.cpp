
#include "Ledger.h"

uint256 Ledger::getAccountRootIndex(const uint160& uAccountID)
{
	Serializer	s;

	s.add16(spaceAccount);
	s.add160(uAccountID);

	return s.getSHA512Half();
}

// What is important:
// --> uNickname: is a Sha256
// <-- SHA512/2: for consistency and speed in generating indexes.
uint256 Ledger::getNicknameIndex(const uint256& uNickname)
{
	Serializer	s;

	s.add16(spaceNickname);
	s.add256(uNickname);

	return s.getSHA512Half();
}

uint256 Ledger::getGeneratorIndex(const uint160& uGeneratorID)
{
	Serializer	s;

	s.add16(spaceGenerator);
	s.add160(uGeneratorID);

	return s.getSHA512Half();
}

uint256 Ledger::getRippleStateIndex(const NewcoinAddress& naA, const NewcoinAddress& naB, const uint160& uCurrency)
{
	uint160		uAID	= naA.getAccountID();
	uint160		uBID	= naB.getAccountID();
	bool		bAltB	= uAID < uBID;
	Serializer	s;

	s.add16(spaceRipple);
	s.add160(bAltB ? uAID : uBID);
	s.add160(bAltB ? uBID : uAID);
	s.add160(uCurrency);

	return s.getSHA512Half();
}

uint256 Ledger::getRippleDirIndex(const uint160& uAccountID)
{
	Serializer	s;

	s.add16(spaceRippleDir);
	s.add160(uAccountID);

	return s.getSHA512Half();
}

uint160 Ledger::getOfferBase(const uint160& currencyIn, const uint160& accountIn,
	const uint160& currencyOut, const uint160& accountOut)
{
	bool inNative = currencyIn.isZero();
	bool outNative = currencyOut.isZero();

	if (inNative && outNative)
		throw std::runtime_error("native to native offer");

	Serializer s(80);

	if (inNative)
	{
		if (!currencyIn) throw std::runtime_error("native currencies are untied");
		s.add32(0); // prevent collisions by ensuring different lengths
	}
	else
	{
		if (!!currencyIn) throw std::runtime_error("national currencies must be tied");
		s.add160(currencyIn);
		s.add160(accountIn);
	}

	if (outNative)
	{
		if (!currencyOut) throw std::runtime_error("native currencies are untied");
	}
	else
	{
		if (!!currencyOut) throw std::runtime_error("national currencies must be tied");
		s.add160(currencyOut);
		s.add160(accountOut);
	}

	return s.getRIPEMD160();
}

uint256 Ledger::getOfferIndex(const uint160& offerBase, uint64 rate, int skip)
{ // the node for an offer index
	Serializer s;
	s.add160(offerBase);
	s.add64(rate);
	s.add32(skip);
	return s.get256(0);
}

int Ledger::getOfferSkip(const uint256& offerId)
{ // how far ahead we skip if an index node is full
	return *offerId.begin();
}

// vim:ts=4
