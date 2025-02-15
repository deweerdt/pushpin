/*
 * Copyright (C) 2014 Fanout, Inc.
 *
 * This file is part of Pushpin.
 *
 * $FANOUT_BEGIN_LICENSE:AGPL$
 *
 * Pushpin is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Pushpin is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Alternatively, Pushpin may be used under the terms of a commercial license,
 * where the commercial license agreement is provided with the software or
 * contained in a written agreement between you and Fanout. For further
 * information use the contact form at <https://fanout.io/enterprise/>.
 *
 * $FANOUT_END_LICENSE$
 */

#include "zrpcresponsepacket.h"

QVariant ZrpcResponsePacket::toVariant() const
{
	QVariantHash obj;

	if(!id.isEmpty())
		obj["id"] = id;

	obj["success"] = success;

	if(success)
	{
		if(value.type() == QVariant::String)
			obj["value"] = value.toString().toUtf8();
		else
			obj["value"] = value;
	}
	else
	{
		obj["condition"] = condition;

		if(value.isValid())
		{
			if(value.type() == QVariant::String)
				obj["value"] = value.toString().toUtf8();
			else
				obj["value"] = value;
		}
	}

	return obj;
}

bool ZrpcResponsePacket::fromVariant(const QVariant &in)
{
	if(in.type() != QVariant::Hash)
		return false;

	QVariantHash obj = in.toHash();

	if(obj.contains("id"))
	{
		if(obj["id"].type() != QVariant::ByteArray)
			return false;

		id = obj["id"].toByteArray();
	}

	if(!obj.contains("success") || obj["success"].type() != QVariant::Bool)
		return false;
	success = obj["success"].toBool();

	value.clear();
	condition.clear();
	if(success)
	{
		if(!obj.contains("value"))
			return false;
		value = obj["value"];
	}
	else
	{
		if(!obj.contains("condition") || obj["condition"].type() != QVariant::ByteArray)
			return false;
		condition = obj["condition"].toByteArray();

		if(obj.contains("value"))
			value = obj["value"];
	}

	return true;
}
